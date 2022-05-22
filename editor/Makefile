##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2018-2022 Quentin Quadrat <lecrapouille@gmail.com>
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

###################################################
# Location of the project directory and Makefiles
#
P=..
M=$(P)/.makefile

###################################################
# Project definition
#
TARGET = OpenGLCppWrapperEditor
DESCRIPTION = Unity-like editor for $(PROJECT)
BUILD_TYPE = debug

include $(P)/Makefile.common

###################################################
# List of files to compile.
#
OBJS += Editor.o main.o
VPATH += src
INCLUDES += -Isrc

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#FIXME: the static lib name shall be extracted from root makefile
#
THIRDPART_LIBS += $(abspath $(P)/build/libopenglcppwrapper.a.$(shell cat $(P)/VERSION.txt))

###################################################
# Compile tutorials
all: $(THIRDPART_LIBS) $(TARGET) $(RESOURCES_DONE)

###################################################
# Be sure the main project have been compiled
# and assets downloaded
$(OBJS): | $(THIRDPART_LIBS)
$(THIRDPART_LIBS):
	@$(call print-to,"Compiling","$(THIRDPART_LIBS)","$@","$(VERSION)")
	@cd $(P) && $(MAKE)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
