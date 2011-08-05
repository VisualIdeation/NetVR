########################################################################
# Makefile for mesh manipulation program.
# Copyright (c) 2005 Oliver Kreylos
########################################################################

# Set the Vrui installation directory:
VRUIDIR = /usr/local/Vrui-1.0
#VRUIDIR = $(HOME)/Vrui-1.0

# Define Vrui include and link settings:
VRUIINC = -I$(VRUIDIR)/include
ifeq ($(shell uname -m), x86_64)
  LIBEXT = lib64
else
  LIBEXT = lib
endif
VRUILINK = -L$(VRUIDIR)/$(LIBEXT) -lVrui.g++-3 -Wl,-rpath $(VRUIDIR)/$(LIBEXT) -lglut -lGLU

o/%.o: %.cpp
	@mkdir -p o
	@echo Compiling $<...
	@g++ -c -o $@ -g0 -O3 -DNDEBUG $(VRUIINC) $<

SOURCES = caveapp.cpp

Demo: $(SOURCES:%.cpp=o/%.o)
	g++ -o $@ $^ $(VRUILINK)
