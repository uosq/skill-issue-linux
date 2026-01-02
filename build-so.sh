#!/usr/bin/env bash

#    internal/gui.cpp \

g++ -shared -fPIC \
    internal/main.cpp \
    internal/libsigscan.c \
    internal/sdk/definitions/ienginetrace.cpp \
    internal/libdetour/*.c \
    -o build/libvapo.so \
    -O2 -s -std=c++17 \
    `sdl2-config --cflags --libs`