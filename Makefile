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
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
-include $(M)/Makefile.header

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# List of files to compile. Splited by directories
ifeq ($(PROJECT_MODE),debug)
OBJ += backward.o
else
OBJ  =
endif
OBJ += Exception.o PendingData.o
OBJ += GLException.o OpenGL.o GLWindow.o

OBJ += imgui_draw.o imgui_widgets.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 -fPIC
LDFLAGS  =
ARFLAGS  = cr

###################################################
#
INCLUDES += -I$(P)/src -I$(P)/src/private -I$(P)/external -I$(P)/external/imgui
VPATH += $(P)/src:$(P)/src/private:$(P)/external:$(P)/external/imgui

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DERROR -UDEBUG
DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLEW

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS  = $(abspath $(P)/external/SOIL/libSOIL.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw
SO="dylib"

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -lGLU -lGL -lGLEW `pkg-config glfw3 --static --libs`
SO="so"

## Windows
else

SO="dll"
#$(error Unknown architecture)
endif

###################################################
# Address sanitizer. Uncomment these lines if asan
# is desired.
##OPTIM = -O1 -g
##CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
##LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
##LIBS += -static-libstdc++ -static-libasan
##SANITIZER := ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer)

###################################################
# Backward allows tracing stack when segfault happens
ifeq ($(PROJECT_MODE),debug)
ifneq ($(ARCHI),Darwin)
OPTIM_FLAGS = -O2 -g
DEFINES += -DBACKWARD_HAS_DW=1
LIBS += -ldw
endif
else
OPTIM_FLAGS = -O3
endif

###################################################
# Alias names
# TARGET_LIB="lib"$(TARGET)_$(shell uname -m)-$(shell uname -s)
TARGET_LIB=lib$(TARGET)
STATIC_TARGET=$(TARGET_LIB).a
SHARED_TARGET=$(TARGET_LIB).$(SO).$(TARGET_VERSION)
SYMBOLIC_SHARED_TARGET=$(TARGET_LIB).$(SO).$(TARGET_MAJOR_VERSION)

###################################################
#
all: $(STATIC_TARGET) $(SHARED_TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Static library
$(STATIC_TARGET): $(OBJ)
	@$(call print-to,"Static lib","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(AR) $(ARFLAGS) $(STATIC_TARGET) $(OBJ) $(abspath $(P)/external)/SOIL/obj/*.o && ranlib $(STATIC_TARGET)

###################################################
# Shared library
$(SHARED_TARGET): LDFLAGS += -shared
$(SHARED_TARGET): $(OBJ)
	@$(call print-to,"Shared lib","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(LDFLAGS) $(OBJ) $(LIBS) -o $(SHARED_TARGET)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(OPTIM_FLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Download external projects that OpenGLCppWrapper needs.
.PHONY: download-external-libs
download-external-libs:
	@cd external && ./download-external-libs.sh $(ARCHI); cd .. > /dev/null 2> /dev/null

###################################################
# Compile external projects that OpenGLCppWrapper needs.
.PHONY: compile-external-libs
compile-external-libs:
	@cd external && ./compile-external-libs.sh $(ARCHI); cd .. > /dev/null 2> /dev/null

###################################################
# https://scan.coverity.com/
# Coverity Scan: static analysis of code (web service)
# For working, this service needs you download a runnable
# and to compile your code with it. Once done, you have to
# create a tarball of generated files and to upload the
# tarball to the website.
#
# Compile again the project for Coverity Scan. An uploadable tarball is created.
.PHONY: coverity-scan
coverity-scan: clean
	@rm -fr OpenGLCppWrapper.tgz cov-int 2> /dev/null
	@cov-build --dir cov-int make -j8 && tar czvf OpenGLCppWrapper.tgz cov-int

###################################################
# Compile and launch unit tests. Then generate the html code coverage.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests coverage

###################################################
# Launch the executable with address sanitizer (if enabled).
.PHONY: run
run: $(TARGET)
	$(SANITIZER) ./build/$(TARGET) 2>&1 | ./external/asan_symbolize.py

###################################################
# Generate the code source documentation with doxygen.
.PHONY: doc
doc:
	@doxygen Doxyfile
	@xdg-open doc/html/index.html >/dev/null

###################################################
# Compress OpenGLCppWrapper sources without its .git, build
# folders and doc generated files. If a tarball
# already exists, the older will stay intact and a
# new one is created. Tarball confict names is managed.
#
# Compress the code source into a unique tarball for backup.
.PHONY: targz
targz:
	@./.makefile/targz.sh $(PWD)

###################################################
# Create an uploadable tarball for the OpenSuse Build Service (OBS).
.PHONY: obs
obs:
	@./.integration/opensuse-build-service.sh

###################################################
# Install project. You need to be root user.
.PHONY: install
install: $(STATIC_TARGET) $(SHARED_TARGET)
	@$(call print-to,"Installing","doc","$(PROJECT_DOC_PATH)","")
	@mkdir -p $(PROJECT_DOC_PATH)
	@cp -r doc/* $(PROJECT_DATA_ROOT)/doc
	@cp AUTHORS LICENSE README.md ChangeLog $(PROJECT_DATA_ROOT)
	@$(call print-to,"Installing","libs","$(STATIC_TARGET)","")
	@cp build/$(STATIC_TARGET) $(LIBDIR)
	@cp build/$(SHARED_TARGET) $(LIBDIR)
	@ln -s $(LIBDIR)/$(SHARED_TARGET) $(LIBDIR)/$(SYMBOLIC_SHARED_TARGET)

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
include $(M)/Makefile.help
include $(M)/Makefile.footer
