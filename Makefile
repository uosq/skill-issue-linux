CC_CPP = g++
CC_C = gcc

# Make the number of jobs equal the number of threads the CPU is capable of
MAKEFLAGS := --jobs=$(shell nproc)

# Compiler flags
CFLAGS = -march=x86-64-v3 -shared -std=c++17 -O2 -fPIC -Werror -g
CFLAGS_C = -march=x86-64-v3 -O2 -fPIC -Werror -g

# Linker flags
LDFLAGS = -lSDL2 -lvulkan -lm -ldl
LDFLAGS += $(shell find build/ -name '*.a')

# Source files
CPP_FILES = $(shell find src/ -name '*.cpp')
C_FILES = $(shell find src/ -name '*.c')

# Object files
OBJ_CPP = $(addprefix obj/, $(CPP_FILES:=.o))
OBJ_C = $(addprefix obj/, $(C_FILES:=.o))
OBJS = $(OBJ_CPP) $(OBJ_C)

# Binary
BIN = build/libvapo.so

.PHONY: all clean debug

#-------------------------------------------------------------------------------

all: $(BIN)

clean:
	rm -f $(OBJS)
	rm -f $(BIN)

#-------------------------------------------------------------------------------

$(BIN): $(OBJS)
	$(CC_CPP) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# C++ compilation
obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC_CPP) $(CFLAGS) -c -o $@ $<

# C compilation
obj/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC_C) $(CFLAGS_C) -c -o $@ $<