#!/usr/bin/env bash

if [ ! -d build ]; then
	mkdir build
fi

# check for Pluto-lang static binary
# if not there, compile it
if [ ! -f build/libplutostatic.a ]; then
	echo "Compiling Pluto"

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

if [ ! -f build/libsigscan.a ]; then
	echo "Compiling libsigscan"
	gcc -c src/libsigscan.c -o build/libsigscan.o
	ar rcs build/libsigscan.a build/libsigscan.o
fi

if [ ! -f build/libdetour.a ]; then
	echo "Compiling libdetour"
	gcc -c src/libdetour/libdetour.c -o build/libdetour.o
	ar rcs build/libdetour.a build/libdetour.o
fi

# copy our p100 attach script
cp attach.sh build/

chmod +x build/attach.sh

echo "Compiling skill issue"

# compile it
# this shit takes longer to compile than I want to admit
# i shouldn't include the entire pluto and glew libraries
# but it doesn't attach without them
# fuck my life
g++ -shared -fPIC \
	$(find src -name "*.cpp") \
	build/libdetour.a \
	build/libsigscan.a \
	build/libplutostatic.a \
	-o build/libvapo.so \
	-O2 -std=c++17 -lSDL2 -lvulkan -lm -ldl \
	-Werror -fno-exceptions -s -march=x86-64-v3