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

PROJECT = OpenGLCppWrapper
TARGET = $(PROJECT)
PROJECT_MODE = debug

P=.
M=$(P)/.makefile
include $(M)/Makefile.header

CXXFLAGS = -W -Wall -Wextra -O3 -std=c++11
LDFLAGS  =

DEFINES += -DCHECK_OPENGL -DERROR -UDEBUG
DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLEW

OBJ += Exception.o PendingData.o
OBJ += GLException.o OpenGL.o GLWindow.o
OBJ += imgui_draw.o imgui_widgets.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o

INCLUDES += -Isrc -Iexternal -Iexternal/imgui
VPATH += src:src/private:external:external/imgui

EXTERNAL_OBJ += $(abspath external)/SOIL/obj/*.o

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

include $(M)/Makefile.footer
