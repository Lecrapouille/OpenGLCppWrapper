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
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Location from the project root directory.
P=..

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
CXXFLAGS = $(CXX_WHOLE_FLAGS) -std=c++11
LDFLAGS =

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
# Inform Makefile where to find header files
INCLUDES += -I$(P)/src -I$(P)/src/private -I $(P)/external

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH += $(P)/src:$(P)/src/private:$(P)/external

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DARCHI=$(ARCHI)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#FIXME should be $(STATIC_LIB_TARGET) but referring to the main project ==> $(STATIC_LIB_MAIN_TARGET)
#FIXME idem for $(BUILD) ==> $(MAIN_BUILD)
EXTERNAL_LIBS = \
	$(abspath $(P)/external/SOIL/libSOIL.a) \
	$(abspath $(P)/$(BUILD)/libOpenGLCppWrapper.a)

###################################################
# But sure the main project have been compiled
ifeq ("$(wildcard $(P)/$(BUILD)/libOpenGLCppWrapper.a)","")
$(error Please compile the main project before compiling unit tests: cd $(P) && make)
endif

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

ifeq ($(ARCHI),Darwin)
EXTERNAL_LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw
else ifeq ($(ARCHI),Linux)
EXTERNAL_LIBS += -lGLU -lGL -lGLEW `pkg-config glfw3 --static --libs` -lcrpcut -ldl
else
$(error Unknown architecture)
endif

###################################################
# Compile OpenGLCppWrapper unit tests. Be sure libs
# of the main project have been compiled.
all: $(TARGET)

###################################################
# Run unit tests.
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@$(SANITIZER) ./$(BUILD)/$(TARGET) || (cat $(TARGET).log; return 1)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) $(DOC) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
