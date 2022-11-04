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
CC="$3"
CXX="$4"

function print-compile
{
    echo -e "\033[35m*** Compiling:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

### Number of CPU cores
NPROC=
if [[ "$ARCHI" == "Darwin" ]]; then
    NPROC=`sysctl -n hw.logicalcpu`
else
    NPROC=`nproc`
fi

### Library SOIL for opening pictures files (jpeg, png ...)
print-compile SOIL
if [ -e SOIL ];
then
    (cd SOIL
     VERBOSE=1 make -j$NPROC
    )
else
    echo "Failed compiling external/SOIL: directory does not exist"
fi

### Library Bullet3
print-compile bullet3
if [ -e bullet3 ]; then
    (cd bullet3
     rm -fr build usr 2> /dev/null
     mkdir -p build
     cd build
     cmake CXX="$CXX" CC="$CC" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF \
	   -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_EXTRAS=OFF \
	   -DUSE_DOUBLE_PRECISION=OFF -DUSE_GLUT=OFF -DBT_USE_EGL=OFF \
	   -DUSE_OPENVR=OFF -DBUILD_PYBULLET=OFF -DBUILD_PYBULLET_NUMPY=OFF \
	   -DBUILD_ENET=OFF -DBUILD_CLSOCKET=OFF -DUSE_GRAPHICAL_BENCHMARK=OFF \
	   -DBUILD_CPU_DEMOS=OFF -DBUILD_BULLET2_DEMOS=OFF \
	   -DBUILD_UNIT_TESTS=OFF ..
     make -j$NPROC
     make install DESTDIR=..
    )
else
    echo "Failed compiling external/bullet3: directory does not exist"
fi
