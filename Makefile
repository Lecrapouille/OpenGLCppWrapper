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

###################################################
# Do the list of compiled files
OBJ += Exception.o PendingData.o
OBJ += GLException.o OpenGL.o GLWindow.o
OBJ += imgui_draw.o imgui_widgets.o imgui.o \
       imgui_impl_glfw.o imgui_impl_opengl3.o

###################################################
# Compilation options.
CXXFLAGS = $(CXX_WHOLE_FLAGS) -O3 -std=c++11
LDFLAGS  =

###################################################
# Inform Makefile where to find header files
INCLUDES += -Isrc -Iexternal -Iexternal/imgui

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH += src:src/private:external:external/imgui

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DERROR -UDEBUG
DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLEW

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS =
EXTERNAL_OBJ += $(abspath external)/SOIL/obj/*.o

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

ifeq ($(ARCHI),Darwin)
EXTERNAL_LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw
else ifeq ($(ARCHI),Linux)
EXTERNAL_LIBS += -lGLU -lGL -lGLEW `pkg-config glfw3 --static --libs`
else
$(error Unknown architecture)
endif

###################################################
# Compile static and shared libraries
all: $(STATIC_LIB_TARGET) $(SHARED_LIB_TARGET)

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
	@$(call print-to,"Installing","doc","$(PROJECT_DOC_PATH)","")
	@mkdir -p $(PROJECT_DOC_PATH)
	@cp -r doc/* $(PROJECT_DATA_ROOT)/doc
	@cp AUTHORS LICENSE README.md ChangeLog $(PROJECT_DATA_ROOT)
	@$(call print-to,"Installing","libs","$(STATIC_LIB_TARGET)","")
	@cp $(BUILD)/$(STATIC_LIB_TARGET) $(LIBDIR)
	@cp $(BUILD)/$(SHARED_LIB_TARGET) $(LIBDIR)
	@ln -s $(LIBDIR)/$(SHARED_LIB_TARGET) $(LIBDIR)/$(SYMBOLIC_SHARED_TARGET)

###################################################
# Uninstall the project. You need to be root.
.PHONY: uninstall
uninstall:
	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
	@rm $(PROJECT_EXE)
	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the build/ directory.
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) 2> /dev/null

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
