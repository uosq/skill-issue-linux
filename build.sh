#!/usr/bin/env bash

g++ -shared -fPIC main.cpp libsigscan.c -o libvapo.so -std=c++20