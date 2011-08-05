######################################################################
# Copyright (c) 2006-2008 Braden Pellett and Jordan Van Aalsburg
# 
# This file is part of NetVR.
# 
# NetVR is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# NetVR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NetVR.  If not, see <http://www.gnu.org/licenses/>.
######################################################################

######################################################################
# Makefile for Network VR.
######################################################################

#
# System type check
#

ifeq ($(shell uname -s),Darwin)
  SYSTEM_NAME = Darwin
endif
ifeq ($(shell uname -s),Linux)
  SYSTEM_NAME = Linux
endif
ifndef SYSTEM_NAME
  $(error Unsupported system type)
endif


#
# Tools
#

# Compiler
GCC = g++


#
# General information
#

PROJECT_NAME = NetVR


#
# Source files
#

XML_SOURCES = XML/ostream.cpp XML/DocumentBuilder.cpp \
              XML/istream.cpp XML/DocumentDismantler.cpp

NETITOR_SOURCES = Netitor/IDObject.cpp Netitor/Network.cpp Netitor/Node.cpp \
                  Netitor/EdgeLike.cpp Netitor/Edge.cpp \
                  Netitor/DirectedEdge.cpp Netitor/UndirectedEdge.cpp \
                  Netitor/Network.xml.cpp \
                  Netitor/NetworkMonitor.cpp Netitor/NodeMonitor.cpp \
                  Netitor/InteractionMonitor.cpp Netitor/EdgeMonitor.cpp

NETVR_SOURCES = NetVR/PositionedObject.cpp NetVR/Octree.cpp NetVR/ForceOctree.cpp \
                NetVR/StyleSelector.cpp NetVR/Network.cpp NetVR/Node.cpp NetVR/Edge.cpp

GLUTILS_SOURCES = GLUtils/GLUtils.cpp styles.cpp

CLASSTEST_SOURCES = $(XML_SOURCES) $(NETITOR_SOURCES) classtest.cpp

PHYCLASS_SOURCES = $(XML_SOURCES) $(NETITOR_SOURCES) $(NETVR_SOURCES) phyClass.cpp

CAVEAPP_SOURCES = $(XML_SOURCES) $(NETITOR_SOURCES) $(NETVR_SOURCES) $(GLUTILS_SOURCES) \
		class.cpp caveapp.cpp caveapp_ui.cpp caveapp_locator.cpp main.cpp

OCTEST_SOURCES = NetVR/PositionedObject.cpp NetVR/Octree.cpp NetVR/ForceOctree.cpp octest.cpp

ALL_SOURCES = $(CLASSTEST_SOURCES) $(PHYCLASS_SOURCES) $(CAVEAPP_SOURCES) $(OCTEST_SOURCES)


#
# Destination directories
#

BINDIR = bin
OBJDIR = .o
DEPDIR = .d


#
# Flags for compiling and linking
#

# Universally used flags
CFLAGS = -g -Wall -I./
ifeq ($(shell hostname),caveman.geology.ucdavis.edu)
  CFLAGS = -g0 -O3 -mfpmath=sse -msse2 -mmmx -m3dnow -DNDEBUG -Wall -I./
endif
LFLAGS = 

ifeq "$(SYSTEM_NAME)" "Darwin"
	CFLAGS += -D_OLD_VRUI
endif

# link to the expat library
EXPAT_LFLAGS = -lexpat

# Set the Vrui installation directory:
VRUIDIR ?= /opt/local/Vrui-1.0
ifeq ($(shell hostname),caveman.geology.ucdavis.edu)
  VRUIDIR = /usr/local/Vrui-1.0
endif

# Define Vrui include and link settings:
include $(VRUIDIR)/etc/Vrui.makeinclude
VRUI_LINKFLAGS += -lGLU

#
# Dependancy and flag assignments
#

.PHONY: all
all: classtest phyclass caveapp octest

# classtest binary
$(BINDIR)/classtest: $(CLASSTEST_SOURCES:%.cpp=$(OBJDIR)/%.o)
$(BINDIR)/classtest: LFLAGS += $(EXPAT_LFLAGS)
.PHONY: classtest
classtest: $(BINDIR)/classtest

# phyclass binary
$(BINDIR)/phyclass: $(PHYCLASS_SOURCES:%.cpp=$(OBJDIR)/%.o)
$(BINDIR)/phyclass: CFLAGS += $(VRUI_CFLAGS)
$(BINDIR)/phyclass: LFLAGS += $(EXPAT_LFLAGS) $(VRUI_LINKFLAGS)
.PHONY: phyclass
phyclass: $(BINDIR)/phyclass

# caveapp binary
$(BINDIR)/caveapp: $(CAVEAPP_SOURCES:%.cpp=$(OBJDIR)/%.o)
$(BINDIR)/caveapp: CFLAGS += $(VRUI_CFLAGS)
$(BINDIR)/caveapp: LFLAGS += $(EXPAT_LFLAGS) $(VRUI_LINKFLAGS)
.PHONY: caveapp
caveapp: $(BINDIR)/caveapp

# octest binary
$(BINDIR)/octest: $(OCTEST_SOURCES:%.cpp=$(OBJDIR)/%.o)
$(BINDIR)/octest: CFLAGS += $(VRUI_CFLAGS)
$(BINDIR)/octest: LFLAGS += $(VRUI_LINKFLAGS)
.PHONY: octest
octest: $(BINDIR)/octest


#######################
# Generic build rules
#

ifdef SHOWCMDS
  QUIET := 
else
  QUIET := @
endif

$(BINDIR)/%:
	@echo Linking executable $@...
	$(QUIET)mkdir -p $(BINDIR)
	$(QUIET)$(GCC) -o $@ $^ $(LFLAGS)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(ALL_SOURCES:%.cpp=$(DEPDIR)/%.d)
endif

# $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(GCC) -MM -MF $3 -MP -MT $2 $(CFLAGS) $1
endef

$(OBJDIR)/%.o: %.cpp
	@echo Compiling $<...
	$(QUIET)mkdir -p $(OBJDIR)/$(*D)
	$(QUIET)mkdir -p $(DEPDIR)/$(*D)
	$(QUIET)$(call make-depend,$<,$@,$(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d))
	$(QUIET)$(GCC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	@echo Removing object and dependency files...
	$(QUIET)rm -rf $(OBJDIR)/
	$(QUIET)rm -rf $(DEPDIR)/

.PHONY: archive
archive:
	@echo Archiving the project...
	$(QUIET)rm -f $(PROJECT_NAME).tbz
	$(QUIET)tar -cf $(PROJECT_NAME).tar `find . -type f \! \( -path "*/.*" -or -path "*/_*" -or -path "./bin*" -or -path "*~" \)`
ifeq "$(SYSTEM_NAME)" "Darwin"
	$(QUIET)tar --delete -f $(PROJECT_NAME).tar */._*
endif
	$(QUIET)bzip2 $(PROJECT_NAME).tar
	$(QUIET)mv $(PROJECT_NAME).tar.bz2 $(PROJECT_NAME).tbz
