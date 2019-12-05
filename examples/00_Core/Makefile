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
# Location of the project directory and Makefiles
#
P=../..
M=$(P)/.makefile

###################################################
# Project definition
#
TARGET = OpenGLExamples
DESCRIPTION = Unit tests for $(PROJECT)
BUILD_TYPE = debug

include $(P)/Makefile.common

###################################################
# List of files to compile.
#
OBJS += \
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
  12_ComplexShader.o \
  13_Lighting.o \
  14_MapEditor.o \
  15_MaterialBasic.o \
  16_MaterialDepth.o \
  17_MaterialNormal.o \
  main.o

###################################################
# Downoad textures
RESOURCES_DONE=../textures/resources.downloaded

define DOWNLOAD_RESOURCES
	@(cd $(P)/external && ./download-resources.sh && cd - > /dev/null && touch $(RESOURCES_DONE))
endef

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#FIXME: the static lib name shall be extracted from root makefile
#
THIRDPART_LIBS += $(abspath $(P)/build/libopenglcppwrapper.a.$(shell cat $(P)/VERSION))

###################################################
# Compile tutorials
all: $(THIRDPART_LIBS) $(TARGET) $(RESOURCES_DONE)

###################################################
# Be sure the main project have been compiled
# and textures downloaded
$(OBJS): | $(THIRDPART_LIBS)
$(THIRDPART_LIBS):
	@$(call print-to,"Compiling","$(THIRDPART_LIBS)","$@","$(VERSION)")
	@cd $(P) && $(MAKE)

###################################################
# Download textures and other resources needed by examples
.PHONY: download-resources
download-resources: | $(BUILD)
	@$(call DOWNLOAD_RESOURCES)

$(RESOURCES_DONE): | $(BUILD)
	@$(call DOWNLOAD_RESOURCES)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
