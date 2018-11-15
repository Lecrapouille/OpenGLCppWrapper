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

# This script, called by Makefile, makes a backup of the code source as
# an unique tarball. Tarballs name are unique OpenGLCppWrapper$DATE-v$NTH.tar.gz
# where $DATE is the date of today and $NTH a counter to make unique if
# a tarball already exists. Some informations are not backuped like .git
# generated doc, builds ...

# $1 is the content of pwd.

#set -x

HERE=${1##*/}
DATE='-'`date +%Y-%m-%d`
VERSION='-'`cat VERSION`
NTH=0
TARGET_TGZ="OpenGLCppWrapper$DATE-v$NTH.tar.gz"

### Iterate for finding an unique name
while [ -e "$TARGET_TGZ" ];
do
    NTH=`echo "$TARGET_TGZ" | cut -d"v" -f2 | cut -d"." -f1`
    if [ "$NTH" == "" ];
    then
        echo "ERROR: cannot manage this case $TARGET_TGZ"
        exit 1
    else
        NTH=$(( NTH + 1 ))
        TARGET_TGZ="OpenGLCppWrapper$DATE-v$NTH.tar.gz"
    fi
done

### Display informations with the same look than Makefile
echo -e "\033[35m*** Tarball:\033[00m \033[36m$TARGET_TGZ\033[00m <= \033[33m$1\033[00m"

### Compress ../OpenGLCppWrapper in /tmp, append the version number to the name and move the
### created tarball from /tmp into the project root directory.
(cd .. && tar --transform s/OpenGLCppWrapper/OpenGLCppWrapper$VERSION/ \
              --exclude='.git' --exclude="OpenGLCppWrapper-*.tar.gz" --exclude="doc/coverage" \
              --exclude "doc/coverage" --exclude "*/build" -czvf /tmp/$TARGET_TGZ $HERE \
              > /dev/null && mv /tmp/$TARGET_TGZ $HERE)

cp $TARGET_TGZ OpenGLCppWrapper$VERSION.tar.gz
