#!/bin/bash

xhost +local:

mkdir -p /tmp/miceos-docker/share
mkdir -p /tmp/miceos-docker/xdg

chmod -R a+rwx /tmp/miceos-docker

docker run \
-ti --rm \
--net=host \
-e DISPLAY=$DISPLAY \
-e XDG_RUNTIME_DIR=/tmp/miceos-docker/xdg \
--device /dev/dri \
--name miceos-docker-instance \
-v /dev/shm:/dev/shm \
-v /tmp/miceos-docker/share:/mnt/host \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /tmp/miceos-docker/xdg:/tmp/miceos-docker/xdg \
-v `pwd`:/home/developer/miceOS \
filcuc/miceos-docker \
/bin/bash -c "\
cd /home/developer/miceOS/;\
cp .*.user CMakeLists.txt.user;\
/opt/qtcreator/bin/qtcreator /home/developer/miceOS/CMakeLists.txt"

xhost -local:

