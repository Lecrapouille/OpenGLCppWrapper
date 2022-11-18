#!/bin/bash -e

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
TARGET="$2"

### Delete all previous directories to be sure to have and compile
### fresh code source.
rm -fr backward-cpp imgui SOIL units bullet3 json 2> /dev/null

function cloning
{
    REPO="$1"
    shift

    echo -e "\033[35m*** Cloning: \033[36mhttps://github.com/$REPO\033[00m >= \033[33m$TARGET\033[00m"
    git clone https://github.com/$REPO.git --depth=1 $* > /dev/null
}

### Library for real-time collision detection and multi-physics simulation
### License: zlib
cloning bulletphysics/bullet3 --branch 3.24

### Library SOIL for opening pictures files (jpeg, png ...)
### License: MIT
cloning Lecrapouille/SOIL

#### Library for creating GUI in OpenGL
#### License: MIT
cloning ocornut/imgui --branch docking

#### Library for unit conversion library
#### License: MIT
cloning nholthaus/units
(cd units/include && mkdir units && cp units.h units/units.hpp)

#### JSON for Modern C++
#### License: MIT
cloning nlohmann/json
