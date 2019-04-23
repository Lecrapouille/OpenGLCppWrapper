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
TARGET = $(PROJECT)
DESCRIPTION = C++11 API wrapping Core Profile OpenGL routines and allowing to write applications in few lines of code

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# Make the list of compiled files
OBJ_CORE = Exception.o OpenGL.o GLWindow.o
OBJ_IMGUI = imgui_draw.o imgui_widgets.o imgui.o \
            imgui_impl_glfw.o imgui_impl_opengl3.o
OBJ += $(OBJ_CORE) $(OBJ_IMGUI)

###################################################
# Compile static and shared libraries
all: $(STATIC_LIB_TARGET) $(SHARED_LIB_TARGET) $(PKG_FILE)

###################################################
# Download textures and other resources needed by examples
.PHONY: download-resources
download-resources:
	@(cd external && ./download-resources.sh)

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests coverage

###################################################
# Install project. You need to be root.
.PHONY: install
install: $(STATIC_LIB_TARGET) $(SHARED_LIB_TARGET)
	@$(call RULE_INSTALL_DOC)
	@$(call RULE_INSTALL_LIBRARIES)
	@$(call RULE_INSTALL_HEADER_FILES)
	@$(call RULE_INSTALL_PKG_CONFIG)

###################################################
# Uninstall the project. You need to be root. FIXME: to be updated
.PHONY: uninstall
uninstall:
	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
	@rm $(PROJECT_EXE)
	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the whole project.
.PHONY: veryclean
veryclean: clean
	@rm -fr cov-int OpenGLCppWrapper.tgz *.log foo 2> /dev/null
	@cd tests && make -s clean; cd - > /dev/null
	@cd src/common/graphics/OpenGL/examples/ && make -s clean; cd - > /dev/null
	@cd src/forth/standalone && make -s clean; cd - > /dev/null
	@cd src/core/standalone/ClassicSpreadSheet && make -s clean; cd - > /dev/null
	@$(call print-simple,"Cleaning","$(PWD)/doc/html")
	@cd doc/ && rm -fr html

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
