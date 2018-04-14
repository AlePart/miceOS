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
--security-opt seccomp=unconfined \
-v /dev/shm:/dev/shm \
-v /tmp/miceos-docker/share:/mnt/host \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /tmp/miceos-docker/xdg:/tmp/miceos-docker/xdg \
-v `pwd`:/home/developer/miceOS \
filcuc/miceos-docker \
/bin/bash -c "\
chuid.sh developer $USER_UID $USER_GID /home;\
runuser -l developer -c 'cp /home/developer/miceOS/.docker.CMakeLists.txt.user /home/developer/miceOS/CMakeLists.txt.user';\
runuser -l developer -c 'export DISPLAY=$DISPLAY && /opt/qtcreator/bin/qtcreator /home/developer/miceOS/CMakeLists.txt'"

xhost -local:
