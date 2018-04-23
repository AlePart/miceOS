#!/bin/bash

DEV_DIR=/home/developer/miceOS
DEFAULT_USER_FILE=$DEV_DIR/.docker.CMakeLists.txt.user
USER_FILE=$DEV_DIR/CMakeLists.txt.user
PROJECT_FILE=$DEV_DIR/CMakeLists.txt

if [ ! -e "$USER_FILE" ]; then
    cp $DEFAULT_USER_FILE $USER_FILE
fi

/opt/qtcreator/bin/qtcreator $PROJECT_FILE


