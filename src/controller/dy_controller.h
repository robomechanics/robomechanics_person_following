#ifndef DY_CONTROLLER_H
#define DY_CONTROLLER_H

#include <dy.h>
#include <iostream>
#include <string>
using namespace std;

class DyController{
    int pid;
    string hostname;
    DyDataSet *ds;
    float turn;
    float speedinc;
    float des_speed;
    float speed;
    DyHost *supervisor;
    int is_walk, is_sit, is_stand;

public:
    void DyInit();
    int SendtoController(float turnval, float speedincval, int sitval, int standval, int walkval); //speedval refers to speed incrementals 
};

void DyController::DyInit() {
    // dy_data_init();
    dy_init(0, NULL);
    pid = getpid();
    hostname ="xrl3";   //dy_data_get_string(dy_data_retrieve(getenv("XRHEX_ROBOT_NAME")));
    ds = dy_data_path(hostname.c_str());
    turn = 0;
    
    speedinc = 0;
    des_speed = 0.5;      // desired speed
    speed = 0;

    supervisor = dy_network_connect(hostname.c_str(), 8650);
    if (hostname == "")
    	dy_error("XRHEX_ROBOT_NAME not defined!\n");

    is_walk = 0;
}

int DyController::SendtoController(float turnval, float speedincval, int sitval, int standval, int walkval) {
    /*
    char python_command[200];
    std::string controller_file="/home/robot/proj/xrhex_software/scripts/detector_controller.py"; 
    sprintf(python_command,"%s %f %f %d %d %d", controller_file.c_str(), turnval, speedval, sitval, standval, walkval); 
    system(python_command);
    */

    // dy_network_pull_from(supervisor,"%s.gait.turn", hostname.c_str());
    // dy_network_pull_from(supervisor,"%s.stair.override", hostname.c_str());

    DyData *dyturn, *dyover, *dyspeed;
    dyturn  = dy_data_create(DY_FLOAT,"%s.gait.turn", hostname.c_str());
    dyspeed = dy_data_create(DY_FLOAT,"%s.gait.speed", hostname.c_str());
    dyover  = dy_data_create(DY_UINT8,"%s.stair.override", hostname.c_str());

	if (sitval == 1 && is_sit == 0) {
        dy_signal_send_to(supervisor, "stairclimber_stop");
        dy_signal_send_to(supervisor, "gaitrunner_stop");
        dy_signal_send_to(supervisor, "sit_start");
		dy_data_set_int(dyover, 1);
		dy_network_push_to(supervisor,"%s.stair.override", hostname.c_str());
		dy_debug(1,"Too near. Sit still while waiting for detection\n");
        is_walk = 0;
        is_sit = 1;
        is_stand = 0;
	}
    
    if (standval == 1 && is_stand == 0) {
        dy_signal_send_to(supervisor, "stairclimber_stop");
        dy_signal_send_to(supervisor, "gaitrunner_stop");
        dy_signal_send_to(supervisor, "stand_start");
		dy_data_set_int(dyover, 1);
		dy_network_push_to(supervisor,"%s.stair.override", hostname.c_str());
		dy_debug(1,"Too near. Stand still while waiting for detection\n");
        is_walk = 0;
        is_sit = 0;
        is_stand = 1;
	}

    if (speedincval != 0){
        if (speedinc == 0) {
            speedinc = speedincval;
            des_speed += speedinc * 0.1;
            if (des_speed <= 0.0001)
                des_speed = 0.0;
            printf("desired speed: %.5f", des_speed);
            speed = des_speed;
            dy_data_set_float(dyspeed, speed, 0);
            dy_network_push_to(supervisor,"%s.gait.speed", hostname.c_str());
            dy_debug(1,"speed now: %f\n", speed);
        }
    }else{
        speedinc = 0;
    }

    if (walkval == 1 && is_walk == 0){
        dy_signal_send_to(supervisor, "stairclimber_stop");
        dy_signal_send_to(supervisor, "gaitrunner_start");
        dy_signal_send_to(supervisor, "stand_start");
		dy_data_set_int(dyover, 0);
		dy_network_push_to(supervisor,"%s.stair.override", hostname.c_str());
		dy_debug(1,"Walking\n");
        is_walk = 1;
        is_sit = 0;
        is_stand = 0;

        speed = des_speed;
        dy_data_set_float(dyspeed, speed, 0);
        dy_network_push_to(supervisor,"%s.gait.speed", hostname.c_str());
        dy_debug(1,"speed now: %f\n", speed);
    }


    if (abs(turnval) > 0.10){
        if (abs(turn-turnval) > 0.05){
            turn = turnval;
    	    dy_data_set_float(dyturn, turnval, 0);
    	    dy_network_push_to(supervisor,"%s.gait.turn", hostname.c_str());
    	    dy_debug(1,"turn %f\n", turn);
        }
	}else{
        if (abs(turn) > 0.10){
            turn = 0.0;
            dy_data_set_float(dyturn, turn, 0);
    	    dy_network_push_to(supervisor,"%s.gait.turn", hostname.c_str());
            dy_debug(1,"turn %f\n", turn);
        }
    }

	return 1;
}

#endif //DY_CONTROLLER_H