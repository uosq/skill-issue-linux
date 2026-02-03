#!/usr/bin/env bash

if [ ! -d build ]; then
	mkdir build
fi

# check for Pluto-lang static binary
# if not there, compile it
if [ ! -f build/libplutostatic.a ]; then
	cd build

	wget https://github.com/PlutoLang/Pluto/archive/refs/tags/0.12.2.zip
	unzip "0.12.2.zip"
	rm "0.12.2.zip"

	cd Pluto-0.12.2

	make -j PLAT=linux

	cp src/libplutostatic.a ../

	cd ../
	rm -fr Pluto-0.12.2
	cd ../
fi

# copy our p100 attach script
cp attach.sh build/

chmod +x build/attach.sh

# compile it
# this shit takes longer to compile than I want to admit
# i shouldn't include the entire pluto and glew libraries
# but it doesn't attach without them
# fuck my life
g++ -shared -fPIC \
	src/*.cpp \
	src/libsigscan.c \
	src/libdetour/libdetour.c \
	src/imgui/*.cpp \
	src/sdk/definitions/*.cpp \
	src/features/lua/*.cpp \
	src/sdk/interfaces/*.cpp \
	src/gui/*.cpp \
	src/features/entitylist/*.cpp \
	src/features/aimbot/aimbot.cpp \
	src/features/triggerbot/triggerbot.cpp \
	build/libplutostatic.a \
	-o build/libvapo.so \
	-O2 -std=c++17 -lSDL2 -lvulkan -lm -ldl \
	-Werror -flto=auto -fno-exceptions -s -march=x86-64-v3