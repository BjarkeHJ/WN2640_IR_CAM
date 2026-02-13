#!/bin/bash

PREFIX="ir_image_bag"
TIMESTAMP=$(date +%Y-%m-%d_%H-%M-%S)
OUTPUT_FOLDER="${PREFIX}_${TIMESTAMP}"
TOPICS="/image_raw"

cd ~/WN2640_IR_CAM/scripts
ros2 bag record $TOPICS -o "$OUTPUT_FOLDER"
