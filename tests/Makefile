##=====================================================================
## OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
## Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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
TARGET = $(PROJECT)-UnitTest
DESCRIPTION = Unit tests for $(PROJECT)
BUILD_TYPE = debug
USE_COVERAGE = 1

include $(P)/Makefile.common

###################################################
# List of files to compile.
#
OBJS += VectorTests.o MatrixTests.o
OBJS += QuaternionTests.o TransformationTests.o TransformableTests.o
OBJS += ComponentTests.o
OBJS += PendingDataTests.o PendingContainerTests.o
OBJS += GLObjectTests.o GLShadersTests.o GLProgramTests.o
OBJS += main.o

VPATH += $(P)/tests $(P)/tests/Components $(P)/tests/Common $(P)/tests/Math $(P)/tests/OpenGL
INCLUDES += -I$(P)/tests

###################################################
# Project defines
#
DEFINES += -Wno-unused-function -Wno-undef -Wno-keyword-macro -Wno-float-equal \
  -DPROJECT_DATA_PATH=\"$(PROJECT_DATA_ROOT)/core\"

###################################################
# Compilation options.
#
PKG_LIBS += gtest gmock

###################################################
# Compile SimTaDyn unit tests
all: $(TARGET)

###################################################
# Run unit tests.
.PHONY: unit-tests
.PHONY: check
unit-tests check: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./$(BUILD)/$(TARGET) $(TU_OPTIONS) || (cat $(TARGET).log; return 1)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
