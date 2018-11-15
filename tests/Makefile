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

###################################################
# Executable name
PROJECT = OpenGLCppWrapper
TARGET = $(PROJECT)-UnitTest

###################################################
# Location from the project root directory.
P=..
ABSP=$(abspath $(P))

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header

###################################################
# List of files to compile.
OBJ  = VectorTests.o MatrixTests.o
OBJ += IGLObjectTests.o GLVAOTests.o GLVBOTests.o
OBJ += GLShadersTests.o GLProgramTests.o
OBJ += UnitTests.o

###################################################
# Compilation options.
OPTIM_FLAGS = -O2 -g
CXXFLAGS = -W -Wall -Wextra -std=c++11 
LDFLAGS =

###################################################
# Inform Makefile where to find header files
INCLUDES += -I$(BUILD) -I.. -I../src -I../src/private

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):../src:../src/private

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DARCHI=$(ARCHI)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS = $(abspath $(P)/external/SOIL/libSOIL.a) \
       $(abspath $(P)/build/libOpenGLCppWrapper.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -lGLU -lGL -lGLEW `pkg-config glfw3 --static --libs` -lcrpcut -ldl

## Windows
else

#$(error Unknown architecture)
endif

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
# Address sanitizer. Comment these lines if asan is
# not desired.

# FIXME: clang on Travis-CI bugs during linking (infinite loop).
# FIXME: g++ on Tracis-CI I have a segfault and trace are not human
# readable. Temporary disable asan with Travis

# ifeq ($(CXX),g++)
#ifneq ($(CI),true)
#OPTIM_FLAGS = -O1 -g
#CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
#LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
#LIBS += -static-libstdc++ -static-libasan
#SANITIZER := ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer)
#endif

###################################################
# Compile OpenGLCppWrapper unit tests
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile unit tests
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Run unit tests.
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./build/$(TARGET) || (cat OpenGLCppWrapper.log; return 1)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) $(DOC) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
-include $(M)/Makefile.footer
