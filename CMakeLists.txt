cmake_minimum_required(VERSION 2.8)
project(GOTURN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No build type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

find_package(Boost COMPONENTS system filesystem regex REQUIRED)
find_package(Protobuf REQUIRED)
if(${PROTOBUF_FOUND})
    message("!!found Google Proto Buffer library.")
else()
    message("!!Google Proto Buffer library not found.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(TinyXML REQUIRED)

find_package( OpenCV REQUIRED )
message("Open CV version is ${OpenCV_VERSION}")

find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
# Note: If can't find CUDA, please uncomment the below line and set the path manually
# set(CUDA_INCLUDE_DIRS /path_to_cuda/include)

find_package(Caffe REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})    # ex. -DCPU_ONLY
message("Caffe_DIR is ${Caffe_DIR}")	#specify Caffe_DIR in /cmake/Modules/FindCaffe.cmake
# Note: If can't find Caffe, please uncomment the below line and set the path manually
# set(Caffe_DIR /path_to_caffe/build/install)
# set(Caffe_INCLUDE_DIRS /path_to_caffe/build/install/include)

set(GLOG_LIB glog)
find_package(gflags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIR})

# use dynamism to run python script
set(DY_INCLUDE_DIR /home/robot/proj/dynamism/include)
set(DY_LIBS /home/robot/proj/dynamism/lib)
include_directories(${DY_INCLUDE_DIR})
link_directories(${DY_LIBS})

# add_definitions( "-shared" )

file (GLOB_RECURSE SOURCE_FILES
"src/helper/*.cpp"
"src/train/example_generator.cpp"
"src/train/tracker_trainer.cpp"
"src/loader/*.cpp"
"src/network/*.cpp"
"src/tracker/*.cpp"
"src/native/vot.cpp"

"src/helper/*.h"
"src/train/example_generator.h"
"src/train/tracker_trainer.h"
"src/loader/*.h"
"src/network/*.h"
"src/tracker/*.h"
"src/native/vot.h"
"src/controller/*.h"
)

add_library (${PROJECT_NAME} ${SOURCE_FILES})
# Add src to include directories.
include_directories(src)
include_directories(src/native)

# Add all files.
#file(GLOB_RECURSE hdrs src/*.h*)
#file(GLOB_RECURSE srcs src/*.cpp)
#add_library (${PROJECT_NAME} ${srcs} ${hdrs})

# add_executable (test_tracker_vot src/test/test_tracker_vot.cpp)
# target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
# target_link_libraries (test_tracker_vot ${PROJECT_NAME})
# Note: If can't find trax, please download trax and build it, then uncomment the below line and set the path manually
# target_link_libraries(${PROJECT_NAME} /path_to_trax/build/libtrax.so)

add_executable(ssd_detect src/ssd/detect.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${Caffe_LIBRARIES} ${TinyXML_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES} ${GFLAGS_LIBRARIES})
target_link_libraries (ssd_detect ${PROJECT_NAME} libdynamism.so)

add_executable (test_tracker_alov src/test/test_tracker_alov.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
target_link_libraries (test_tracker_alov ${PROJECT_NAME})

add_executable (save_videos_vot src/test/save_videos_vot.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
target_link_libraries (save_videos_vot ${PROJECT_NAME})

add_executable (train src/train/train.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${Caffe_LIBRARIES} ${TinyXML_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
target_link_libraries (train ${PROJECT_NAME})

add_executable (show_tracker_vot src/visualizer/show_tracker_vot.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Boost_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
target_link_libraries (show_tracker_vot ${PROJECT_NAME})

add_executable (show_tracker_alov src/visualizer/show_tracker_alov.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Caffe_LIBRARIES} ${Boost_LIBRARIES} ${GLOG_LIB} ${PROTOBUF_LIBRARIES})
target_link_libraries (show_tracker_alov ${PROJECT_NAME})

add_executable (show_imagenet src/visualizer/show_imagenet.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${TinyXML_LIBRARIES} ${PROTOBUF_LIBRARIES})
target_link_libraries (show_imagenet ${PROJECT_NAME})

add_executable (show_alov src/visualizer/show_alov.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES})
target_link_libraries (show_alov ${PROJECT_NAME})


