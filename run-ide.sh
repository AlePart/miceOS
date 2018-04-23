#!/bin/bash

USER_UID=`id -u`
USER_GID=`id -g`

docker rm miceos-docker-instance

xhost +local:

mkdir -p /tmp/miceos-docker/share
mkdir -p /tmp/miceos-docker/xdg

chmod -R a+rwx /tmp/miceos-docker

docker run \
-ti --rm \
--net=host \
-e DISPLAY=$DISPLAY \
-e XDG_RUNTIME_DIR=/tmp/miceos-docker/xdg \
--privileged \
--name miceos-docker-instance \
--cap-add=SYS_PTRACE \
-v /dev/shm:/dev/shm \
-v /tmp/miceos-docker/share:/mnt/host \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /tmp/miceos-docker/xdg:/tmp/miceos-docker/xdg \
-v `pwd`:/home/developer/miceOS \
filcuc/miceos-docker \
/bin/bash -c "\
chuid.sh developer $USER_UID $USER_GID /home;\
runuser -l developer -c 'export DISPLAY=$DISPLAY && /home/developer/miceOS/.docker.run.ide.sh'"

xhost -local:
