# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION=.out

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

UNITY_ROOT=./unity

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
# CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
# CFLAGS += -Wold-style-definition
#CFLAGS += -Wno-misleading-indentation

TARGET_BASE=test

TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

SRC_FILES=$(UNITY_ROOT)/src/unity.c \
	src/LedDriver.c \
	test/TestLedDriver.c

INC_DIRS=-Isrc -Iinclude -I$(UNITY_ROOT)/src

SYMBOLS=

all: clean default

default: $(SRC_FILES1) $(SRC_FILES2)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	- ./$(TARGET)
	
clean:
	$(CLEANUP) $(TARGET)

ci: CFLAGS += -Werror
ci: default
