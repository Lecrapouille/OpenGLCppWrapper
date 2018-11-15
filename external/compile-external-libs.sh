#!/bin/bash

### This script will git clone some libraries that OpenGLCppWrapper needs and
### compile them. To avoid pollution, they are not installed into your
### environement. Therefore OpenGLCppWrapper Makefiles have to know where to
### find their files (includes and static/shared libraries).

### $1 is given by ../Makefile and refers to the current architecture.
if [ "$1" == "" ]; then
  echo "Expected one argument. Select the architecture: Linux, Darwin or Windows"
  exit 1
fi
ARCHI="$1"
TARGET=OpenGLCppWrapper

### Download is made by ../Makefile
# ./gitclone.sh $1

function print-compile
{
    echo -e "\033[35m*** Compiling:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

### Library SOIL for opening pictures files (jpeg, png ...)
print-compile SOIL
if [ -e SOIL ];
then
    cd SOIL
    (make -j4) > /dev/null 2> /dev/null

    if [ "$ARCHI" == "Linux" ];
    then
        # Move header and static lib in the same location to be indentical than SOIL for Darwin
        cp src/SOIL.h .
        cp lib/libSOIL.a .
    fi
    cd ..
else
    echo "Failed compiling external/SOIL: directory does not exist"
fi
