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

### Delete all previous directories to be sure to have and compile
### fresh code source.
rm -fr backward-cpp imgui SOIL 2> /dev/null

function print-clone
{
    echo -e "\033[35m*** Cloning:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

### Clone Backward tool: A beautiful stack trace pretty printer for C++. No installation
### is needed but some libraries are needed (libdw-dev, binutils-dev, ...).
### License: MIT
print-clone backward-cpp
git clone https://github.com/Lecrapouille/backward-cpp.git --depth=1 > /dev/null 2> /dev/null

### Library SOIL for opening pictures files (jpeg, png ...)
print-clone SOIL
if [ "$ARCHI" == "Darwin" ];
then

    # License: public domain
    git clone https://github.com/childhood/libSOIL.git --depth=1 > /dev/null 2> /dev/null
    mv libSOIL SOIL

else

    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1 > /dev/null 2> /dev/null
    mv Simple-OpenGL-Image-Library SOIL

fi

### Library for creating GUI in OpenGL
### License: MIT
print-clone imgui
git clone https://github.com/ocornut/imgui.git --depth=1 > /dev/null 2> /dev/null
#(cd imgui && git reset --hard f9b873662baac2388a4ca78c967e53eb5d83d2a1)

### Library for unit conversion library
### License: MIT
print-clone units
git clone https://github.com/nholthaus/units.git --depth=1 > /dev/null 2> /dev/null
(cd units/include && mkdir units && cp units.h units/units.hpp)
