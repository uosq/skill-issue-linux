#!/usr/bin/env bash

g++ -shared -fPIC \
	src/main.cpp \
	src/libsigscan.c \
	src/sdk/definitions/ienginetrace.cpp \
	src/libdetour/libdetour.c \
	src/imgui/*.cpp \
	-o build/libvapo.so \
	-O2 -std=c++17 -lSDL2 -lGLEW -lvulkan \
	-Wl,-rpath,'$ORIGIN'