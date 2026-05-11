CC_CPP = g++
CC_C = gcc

# Make the number of jobs equal the number of threads the CPU is capable of
MAKEFLAGS := --jobs=$(shell nproc)

DEBUG_EXT = .debug
DEBUG ?= 0

# Detect AVX2 support
HAS_AVX2 := $(shell grep -q avx2 /proc/cpuinfo && echo 1 || echo 0)

ifeq ($(HAS_AVX2),1)
	DEFAULT_MARCH = x86-64-v3
else
	DEFAULT_MARCH = x86-64
endif

MARCH ?= $(DEFAULT_MARCH)

# Object directory per architecture
OBJ_DIR = obj/$(MARCH)
BUILD_DIR = build/$(MARCH)

# Output binaries
BIN_V3 = build/x86-64-v3/libvapo.so
BIN_COMPAT = build/x86-64/libvapo.so
BIN ?= $(BUILD_DIR)/libvapo.so

ifeq ($(DEBUG), 1)
	OPT = -O0 -fno-omit-frame-pointer -fasynchronous-unwind-tables
else
	OPT = -O2
endif

# Compiler flags
CFLAGS = -march=$(MARCH) -shared -std=c++17 $(OPT) -fPIC -Werror -g -rdynamic -DSOL_ALL_SAFETIES_ON=1
CFLAGS_C = -march=$(MARCH) $(OPT) -fPIC -Werror -g -rdynamic

# Linker flags
LDFLAGS = -lSDL2 -lvulkan -lm -ldl
LDFLAGS += $(shell find $(BUILD_DIR) -name '*.a' 2>/dev/null)

# Source files
CPP_FILES = $(shell find src/ -name '*.cpp')
C_FILES = $(shell find src/ -name '*.c')

# Object files
OBJ_CPP = $(addprefix $(OBJ_DIR)/, $(CPP_FILES:=.o))
OBJ_C = $(addprefix $(OBJ_DIR)/, $(C_FILES:=.o))
OBJS = $(OBJ_CPP) $(OBJ_C)

.PHONY: all clean v3 compat

#-------------------------------------------------------------------------------

all: $(BIN)

v3:
	$(MAKE) MARCH=x86-64-v3 BIN=$(BIN_V3) DEBUG=$(DEBUG)

compat:
	$(MAKE) MARCH=x86-64 BIN=$(BIN_COMPAT) DEBUG=$(DEBUG)

clean:
	rm -rf obj
	rm -rf build/x86-64 build/x86-64-v3

#-------------------------------------------------------------------------------

$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC_CPP) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ifneq ($(DEBUG), 1)
	# Create debug symbols
	objcopy --compress-debug-sections=zlib --only-keep-debug $@ $@$(DEBUG_EXT)

	# Strip binary
	strip --strip-unneeded $@

	# Attach debug symbols
	objcopy --add-gnu-debuglink=$@$(DEBUG_EXT) $@
endif

# C++ compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC_CPP) $(CFLAGS) -c -o $@ $<

# C compilation
$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC_C) $(CFLAGS_C) -c -o $@ $<