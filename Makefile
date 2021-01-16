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
# Project definition
#
TARGET = $(PROJECT)
DESCRIPTION = C++ Wrapper allowing to write OpenGL Core applications in few lines
BUILD_TYPE = debug

###################################################
# Location of the project directory and Makefiles
#
P := .
M := $(P)/.makefile
include $(P)/Makefile.common

###################################################
# Project specific
#
ifeq ($(ARCHI),Darwin)
OBJ_OPENGL_TEXTURES = $(abspath $(THIRDPART)/SOIL/*.o)
else
OBJ_OPENGL_TEXTURES = $(abspath $(THIRDPART)/SOIL/obj/*.o)
endif

OBJ_COMMON = Exception.o
OBJ_OPENGL_CONTEXT = OpenGL.o
OBJ_OPENGL_VARIABLES = Variables.o
OBJ_OPENGL_BUFFERS = EBO.o VBO.o VAO.o
OBJ_OPENGL_TEXTURES += Textures.o
OBJ_OPENGL_SHADERS = Shader.o Program.o
OBJ_GUI = Window.o Application.o DearImGui.o
OBJ_MATERIALS = Material.o DepthMaterial.o NormalsMaterial.o BasicMaterial.o Color.o
OBJ_GEOMETRIES = Model.o Plane.o Tube.o Sphere.o
OBJ_LOADERS = OBJ.o SOIL.o
OBJ_SCENE_GRAPH = SceneTree.o

OBJS += $(THIRDPART_OBJS) $(OBJ_COMMON)
OBJS += $(OBJ_OPENGL_CONTEXT) $(OBJ_OPENGL_VARIABLES)
OBJS += $(OBJ_OPENGL_BUFFERS) $(OBJ_OPENGL_TEXTURES) $(OBJ_OPENGL_SHADERS)
OBJS += $(OBJ_GUI) $(OBJ_LOADERS) $(OBJ_MATERIALS) $(OBJ_GEOMETRIES)
OBJS += $(OBJ_SCENE_GRAPH)

###################################################
# Compile static and shared libraries
all: $(STATIC_LIB_TARGET) $(SHARED_LIB_TARGET) $(PKG_FILE)

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@$(MAKE) -C tests coverage

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: check
check: unit-tests

###################################################
# Install project. You need to be root.
.PHONY: install
install: $(STATIC_LIB_TARGET) $(SHARED_LIB_TARGET) $(PKG_FILE)
	@$(call INSTALL_DOCUMENTATION)
	@$(call INSTALL_PROJECT_LIBRARIES)
	@$(call INSTALL_PROJECT_HEADERS)
	@$(call INSTALL_THIRDPART_FOLDER,SOIL/src,SOIL,-name "*.h")
	@$(call INSTALL_THIRDPART_FILES,imgui,imgui,-name "*.h")

###################################################
# Uninstall the project. You need to be root. FIXME: to be updated
#.PHONY: uninstall
#uninstall:
#	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
#	@rm $(PROJECT_EXE)
#	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the whole project.
.PHONY: veryclean
veryclean: clean
	@rm -fr cov-int $(PROJECT).tgz *.log foo 2> /dev/null
	@(cd tests && $(MAKE) -s clean)
	@(cd examples/ && $(MAKE) -s clean)
	@$(call print-simple,"Cleaning","$(PWD)/doc/html")
	@rm -fr $(THIRDPART)/*/ doc/html 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
