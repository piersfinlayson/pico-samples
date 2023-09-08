#!/bin/bash
set -e
DEV=$1
IMAGE=$2
if [ -z $DEV ] || [ $DEV = "" ] || [ -z $IMAGE ] || [ IMAGE = "" ]; then
  printf "Usage: %s <device> <image (with .uf2 extension)>\n" $0
  printf "  e.g. %s sda mbus\n" $0
  exit 1
fi
MOUNT_DIR=/tmp/mnt-${DEV}1 
DEVICE=/dev/${DEV}1
mkdir -p $MOUNT_DIR
set +e
printf "Ensure not yet mounted\n"
sudo umount $MOUNT_DIR
sudo umount $DEVICE
set -e
printf "Copying %s to %s\n" ${IMAGE} $DEVICE
sudo mount $DEVICE $MOUNT_DIR
sudo cp ${IMAGE} $MOUNT_DIR/
sudo umount $MOUNT_DIR
printf "Copied\n"
sync
rm -fr $MOUNT_DIR
