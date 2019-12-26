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
P := .
M := $(P)/.makefile

###################################################
# Project definition
#
TARGET = $(PROJECT)
DESCRIPTION = C++11 API wrapping Core Profile OpenGL routines and allowing to write applications in few lines of code
BUILD_TYPE = debug

include $(P)/Makefile.common

###################################################
# Make the list of compiled files
#
OBJ_CORE = Verbose.o Exception.o OpenGL.o Window.o
OBJ_IMGUI = DearImGui.o
OBJS += $(OBJ_CORE) $(OBJ_IMGUI)
ifeq ($(ARCHI),Darwin)
THIRDPART_OBJS += $(abspath $(THIRDPART)/SOIL/*.o)
else
THIRDPART_OBJS += $(abspath $(THIRDPART)/SOIL/obj/*.o)
endif

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
	@$(call RULE_INSTALL_DOC)
	@cd examples && for d in `find . -type d`; do install -d --mode 755 "$$d" "$(PROJECT_DATA_ROOT)/examples/$$d"; done
	@cd examples && for f in `find . -type f`; do install -D --mode 644 "$$f" "$(PROJECT_DATA_ROOT)/examples/$$f"; done
	@$(call RULE_INSTALL_LIBRARIES)
	@$(call RULE_INSTALL_HEADER_FILES)
	@cd src && for d in `find . -type d`; do install -d --mode 755 "$$d" "$(INCLDIR)/$(PROJECT)/$$d"; done
	@cd src && for f in `find . -type f -name "*.hpp"`; do install -D --mode 644 "$$f" "$(INCLDIR)/$(PROJECT)/$$f"; done
	@install -d --mode 755 "$(INCLDIR)/$(PROJECT)/SOIL"
	@cd $(THIRDPART)/SOIL/src && for f in `find . -type f -name "*.h"`; do install -D --mode 644 "$$f" "$(INCLDIR)/$(PROJECT)/SOIL/$$f"; done
	@install -d --mode 755 "$(INCLDIR)/$(PROJECT)/imgui"
	@cd $(THIRDPART)/imgui && for f in `find . -maxdepth 1 -mindepth 1 -type f -name "*.h"`; do install -D --mode 644 "$$f" "$(INCLDIR)/$(PROJECT)/imgui/$$f"; done
	@$(call RULE_INSTALL_PKG_CONFIG)

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
