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
PROJECT = OpenGLCppWrapper
TARGET = $(PROJECT)-UnitTest

###################################################
# Debug mode or Release mode
BUILD_TYPE = debug

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
OBJS  = VectorTests.o MatrixTests.o
OBJS += IGLObjectTests.o GLVAOTests.o GLVBOTests.o
OBJS += GLShadersTests.o GLProgramTests.o
OBJS += UnitTests.o

###################################################
# Link against the crpcut unit test lib.
# Note: that crpcut does not provide pkg-config file.
LDFLAGS += -lcrpcut -ldl

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
#
CXXFLAGS += \
  -Wno-inline -Wno-suggest-attribute=noreturn -Wno-stack-protector

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#FIXME: the static lib name shall be extracted from root makefile
THIRDPART_LIBS += $(abspath $(P)/build/libopenglcppwrapper.a.$(shell cat $(P)/VERSION))

###################################################
# Compile OpenGLCppWrapper unit tests.
all: $(THIRDPART_LIBS) $(TARGET)

###################################################
# Be sure the main project have been compiled
$(OBJS): | $(THIRDPART_LIBS)
$(THIRDPART_LIBS):
	@$(call print-to,"Compiling","$(THIRDPART_LIBS)","$@","$(VERSION)")
	@cd $(P) && $(MAKE)

###################################################
# Run unit tests.
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@$(SANITIZER) ./$(BUILD)/$(TARGET) || (cat $(TARGET).log; return 1)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
