#!/bin/bash -e
##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of OpenGLCppWrapper.
##
## OpenGLCppWrapper is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

# Get the number of CPU cores
NPROC=
if [[ "`uname -s`" == "Darwin" ]]; then
    NPROC=`sysctl -n hw.logicalcpu`
else
    NPROC=`nproc`
fi

# Compile the libOpenGLCppWrapper
make download-external-libs
make compile-external-libs
make -j$NPROC

# Compile the OpenGLCppWrapper editor
(cd editor && make -j$NPROC)

# Compile Examples
(cd examples && make -j$NPROC)

# Compile unit test (if desired)
# (cd tests && make coverage -j$NPROC)
