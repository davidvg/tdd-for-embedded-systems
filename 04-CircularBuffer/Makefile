#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = CircularBuffer
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
	../03-FormatOutput/src

TEST_SRC_DIRS = \
	tests

INCLUDE_DIRS = \
  include \
  ../include/util \
  ../03-FormatOutput/include \
  $(SRC_DIRS) \
  $(CPPUTEST_HOME)/include

MOCKS_SRC_DIRS = mocks ../mocks ../03-FormatOutput/mocks

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
