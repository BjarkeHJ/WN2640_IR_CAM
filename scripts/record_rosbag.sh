#!/bin/bash

BAG_PARENT_DIR="WN2640_IR_CAM/scripts/data"
PREFIX="ir_image_bag"
TIMESTAMP=$(date +%Y-%m-%d_%H-%M-%S)
OUTPUT_FOLDER="$BAG_PARENT_DIR/${PREFIX}_${TIMESTAMP}"
TOPICS="/image_raw"

ros2 bag record $TOPICS -o "$OUTPUT_FOLDER"
