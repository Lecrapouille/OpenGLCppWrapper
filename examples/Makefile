##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of OpenGLCppWrapper.
##
## OpenGLCppWrapper is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## OpenGLCppWrapper is distributedin the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

PROJECT = OpenGLExamples
TARGET = $(PROJECT)
PROJECT_MODE = debug

P=..
M=$(P)/.makefile
include $(M)/Makefile.header

CXXFLAGS = $(CXX_WHOLE_FLAGS) -O3 -std=c++11
LDFLAGS  =

DEFINES += -DCHECK_OPENGL -DERROR -UDEBUG
DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLEW

OBJ += Example01.o Example02.o Example03.o Example04.o main.o

INCLUDES += -Isrc -I$(P)/src -I$(P)/external
VPATH += $(P)/src:$(P)/external/imgui

EXTERNAL_LIBS += $(abspath $(P)/build/libOpenGLCppWrapper.a)

ifeq ($(ARCHI),Darwin)
EXTERNAL_LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw
else ifeq ($(ARCHI),Linux)
EXTERNAL_LIBS += -lGLU -lGL -lGLEW `pkg-config glfw3 --static --libs`
else
$(error Unknown architecture)
endif

###################################################
# Compile tutorials
all: $(TARGET)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ $(BUILD) 2> /dev/null

include $(M)/Makefile.footer
