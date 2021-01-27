##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

### This script shell allows to check if each header file of the
### project can be compiled by itself.

OUT=result.txt
echo "" > $OUT

## Search all .hpp files in src and create a dummy c++ file only
## including the hpp file. Compile this generated file. If everything
## is ok g++ shall not produce errors.
for FILE in `find ../src -name "*.hpp" -type f`
do
    # Generate the cpp file including the header file.
    cat << EOF > checkheaders.cpp
    #include <stdio.h>
    #include "$FILE"
    namespace glwrap { void errout(FILE*, char const*, char const*, unsigned int, char const*, ...) { } }
    unsigned char* SOIL_load_image(const char *, int *, int *, int *, int) { return NULL; }
    void SOIL_free_image_data (unsigned char *) {}
    int main() { return 0; }
EOF

    # Compile the generate file.
    g++ -W -Wall --std=c++11 -I../external -I../build -I../include -I../. -I../src \
      -DPROJECT_DATA_PATH=\"FOO\" checkheaders.cpp -o prog > /dev/null 2> foo2

    # Get the g++ errors and warnings.
    RES=`cat foo2`
    if [ "$RES" = "" ];
    then
        echo -e "\033[32mSUCCESS $FILE\033[00m"
    else
        # Save all file and g++ errors in a global result file.
        echo -e "\033[31mFAILURE $FILE\033[00m"
        echo "#####################################" >> $OUT
        echo "FAILURE $FILE" >> $OUT
        echo "$RES" >> $OUT
        echo "" >> $OUT
    fi
done

rm -fr prog checkheaders.cpp 2> /dev/null

## Print the global result file
RES=`cat $OUT` 2> /dev/null
if [ "$RES" = "" ];
then
    exit 0
else
    #cat $OUT
    exit 1
fi
