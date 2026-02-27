CC=g++

# Make the number of jobs equal the number of threads the CPU is capable of
MAKEFLAGS := --jobs=$(shell nproc)

# Compiler flags
CFLAGS = -march=x86-64-v3 -shared -std=c++17 -O2 -fPIC -Werror -fno-exceptions -g

# Linker flags
LDFLAGS = -lSDL2 -lvulkan -lm -ldl  -g
LDFLAGS += $(shell find build/ -name '*.a')

# Our source files
OBJ_FILES = $(shell find src/ -name '*.cpp' | sed -e "s/$$/.o/")

OBJS = $(addprefix obj/, $(OBJ_FILES)) 

# Binaries
BIN = build/libvapo.so

.PHONY: all clean debug

#-------------------------------------------------------------------------------

all: $(BIN)

clean:
	rm -f $(OBJS)
	rm -f $(BIN)

#-------------------------------------------------------------------------------

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
