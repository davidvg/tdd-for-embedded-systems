#---------
# CppUTest Bare Makefile
# 
# This Makefile is from the CppUTest source examples. It's simpler than the
# Makefile from the Github repository example.
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = LightScheduler
MEMORY_MAP_FILE = $(COMPONENT_NAME).map

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc
# CPPUTEST_LDFLAGS=-Xlinker -Map=$(MEMORY_MAP_FILE)

# This line is overriding the default new macros. This is helpful when using
#  std library includes like <list> and other containers so that memory leak
#  detection does not conflict with stl. The header file does two things:
#  - include <list>
#  - include MemoryLeakDetectorNewMacros.h, in $(CPPUTEST_HOME)/include/CppUTest
#  A new header file doing this two things should be created in an included path
# CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include include/ExamplesNewOverrides.h

SRC_DIRS = \
	src \
	mocks
	
TEST_SRC_DIRS = \
	tests

INCLUDE_DIRS = \
  . \
  ../common/include \
  ./include \
  ./mocks \
  $(SRC_DIRS) \
  $(CPPUTEST_HOME)/include \
  $(CPPUTEST_HOME)/include/CppUTest \

MOCKS_SRC_DIRS = 

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
