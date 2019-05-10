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

###################################################
# Executable name
PROJECT = OpenGLExamples
TARGET = $(PROJECT)

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
include $(P)/Makefile.common

###################################################
# List of files to compile.
OBJ += \
  00_EmptyWindowWithIOEvents.o \
  01_EmptyWindowWithImgui.o \
  02_DynamicTriangle.o \
  03_MultiTexturedTriangle.o \
  05_VAOvsVBO.o \
  06_SphereEBO.o \
  07_SkyBoxTextureCube.o \
  08_TerrainTexture3D.o \
  09_SceneGraph.o \
  10_SimpleFrameBuffer.o \
  11_PostProdFrameBuffer.o \
  main.o

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS += $(abspath $(P)/build/libopenglcppwrapper.a)

###################################################
# Compile tutorials
all: $(EXTERNAL_LIBS) $(TARGET)

###################################################
# Be sure the main project have been compiled
$(OBJ): | $(EXTERNAL_LIBS)
$(EXTERNAL_LIBS):
	@$(call print-to,"Compiling","$(EXTERNAL_LIBS)","$@","$(VERSION)")
	@cd $(P) && $(MAKE)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
