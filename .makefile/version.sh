#!/bin/bash
##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

# From the file VERSION and the current git SHA1, this script generates
# the build/version.h with these informations. This project uses them
# in log files, in "about" windows ... This script is called by Makefile.

### Get majpr and minor version from VERSION file (given as $1)
VERSION=`grep "[0-9]\+\.[0-9]\+" $1 2> /dev/null`
if [ "$VERSION" == "" ];
then
  echo "ERROR: VERSION file is missing or badly formed. Abort !"
  exit 1
fi
MAJOR_VERSION=`echo "$VERSION" | cut -d'.' -f1`
MINOR_VERSION=`echo "$VERSION" | cut -d'.' -f2`

### Get git SHA1 and branch
SHA1=`git log 2> /dev/null | head -n1 | cut -d" " -f2`
BRANCH=`git branch 2> /dev/null | head -n1 | cut -d" " -f2`

### Debug
echo "OpenGLCppWrapper version: $MAJOR_VERSION.$MINOR_VERSION"
echo "git: $BRANCH $SHA1"
echo ""

### Save these informations as C++ header file
cat <<EOF >$2
#ifndef VERSION_H_
#  define VERSION_H_

#  define PROJECT_MAJOR_VERSION ${MAJOR_VERSION}u
#  define PROJECT_MINOR_VERSION ${MINOR_VERSION}u
#  define PROJECT_BRANCH "$BRANCH"
#  define PROJECT_SHA1 "$SHA1"

#endif /* VERSION_H_ */
EOF

exit 0
