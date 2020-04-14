PROJECT = OpenGLExample
TARGET = OpenGLExample
DESCRIPTION = Unit tests for $(PROJECT)
BUILD_TYPE = debug

P=../..
M=$(P)/.makefile
include $(M)/Makefile.header

OBJS += 00_BasicWindow.o main.o
PKG_LIBS += openglcppwrapper
VPATH += src
INCLUDES += -Isrc

all: $(TARGET)

include $(M)/Makefile.footer
