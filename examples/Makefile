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
P=..
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
OBJS_WINDOW = \
  00_BasicWindow.o \
  01_BasicWindowIOEvents.o \
  02_BasicWindowImGui.o

OBJS_CORE = \
  00_ColorfulTriangle.o \
  01_DynamicTriangle.o \
  02_TexturedTriangle.o \
  03_MultiTexturedSquare.o \
  04_IndexedQuad.o \
  05_RotatingQuad.o \
  06_IndexedSphere.o \
  07_MultipleObjects.o \
  08_TerrainTexture3D.o \
  09_SkyBoxTextureCube.o \

#  10_SkyBoxShapeCamera.o \
#  11_SimpleFrameBuffer.o \
#  12_PostProdFrameBuffer.o \
#  13_ComplexShader.o \
#  14_BasicLighting.o

OBJS_SCENE = \
  00_DepthMaterial.o \
#  01_BasicMaterial.o \
#  02_NormalsMaterial.o \
#  03_SceneNodes.o \
#  04_SceneGraph.o \
#  05_SceneFog.o \
#  07_AlphaTest.o

#OBJS_THREE = \
#  00_MiscLookAt.o

#  06_SceneLighting.o \

OBJS += \
  $(OBJS_WINDOW) $(OBJS_CORE) $(OBJS_SCENE) $(OBJS_THREE) debug.o main.o

VPATH += \
  00_Window 01_Core 02_Scene 03_ThreeJs

INCLUDES += \
  -I00_Window -I01_Core -I02_Scene -I03_ThreeJs

###################################################
# Downoad textures
RESOURCES_DONE=textures/resources.downloaded

define DOWNLOAD_RESOURCES
	@git clone https://github.com/Lecrapouille/OpenGLCppWrapper-data.git --depth=1 textures > /dev/null 2> /dev/null; touch $(RESOURCES_DONE)
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
