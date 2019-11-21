#!/bin/bash

### This script will download textures needed for examples. Separate git repos allows
### to keep the size of this repo light.

TARGET=OpenGLCppWrapper
function print-clone
{
    echo -e "\033[35m*** Cloning:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

###
print-clone textures
git clone https://github.com/Lecrapouille/OpenGLCppWrapper-data.git --depth=1 > /dev/null 2> /dev/null
rm -fr ../examples/textures/ > /dev/null 2> /dev/null
mv OpenGLCppWrapper-data ../examples/textures/ > /dev/null 2> /dev/null

exit 0
