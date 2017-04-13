#!/bin/bash

if [ -z "$1" ]
  then
    echo "No video_list_file supplied!"
    echo "Usage: bash `basename "$0"` video_list_file output_video_file"
    exit
fi

if [ -z "$2" ]
  then
    echo "No output_video_file supplied!"
    echo "Usage: bash `basename "$0"` video_list_file output_video_file"
    exit
fi

# Choose which GPU the tracker runs on
GPU_ID=0

FOLDER=GOTURN1_test

DEPLOY_PROTO=nets/tracker.prototxt

CAFFE_MODEL=nets/models/pretrained_model/tracker.caffemodel

OUTPUT_FOLDER=nets/tracker_output/$FOLDER

MODEL_FILE='/home/sharon/work/ssd/caffe/models/VGGNet/VOC0712/SSD_300x300/deploy.prototxt'

WEIGHTS_FILE='/home/sharon/work/ssd/caffe/models/VGGNet/VOC0712/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_120000.caffemodel'

# echo "Saving output to " $OUTPUT_FILE

# Run tracker on test set and save vidoes 
build/ssd_detect -file_type=video -gpu_id=$GPU_ID $MODEL_FILE $WEIGHTS_FILE $DEPLOY_PROTO $CAFFE_MODEL $1 $2
# build/ssd_detect -file_type=videos_folder -gpu_id=$GPU_ID $MODEL_FILE $WEIGHTS_FILE $DEPLOY_PROTO $CAFFE_MODEL $1 $2
