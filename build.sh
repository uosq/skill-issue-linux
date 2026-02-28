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

if [ ! -f build/libimgui.a ]; then
	echo "Compiling ImGui"

	mkdir -p build/imgui

	# compile each .cpp into its own .o
	for f in src/imgui/*.cpp; do
		gcc -c "$f" -O2 -I src/imgui -o "build/imgui/$(basename "$f" .cpp).o"
	done

	ar rcs build/libimgui.a build/imgui/*.o
fi

# copy our p100 attach script
cp attach.sh build/

chmod +x build/attach.sh

echo "Compiling skill issue"

# compile the cheat
make # Note: This will echo

# make .debug symbol file
objcopy --only-keep-debug build/libvapo.so build/libvapo.debug
strip --strip-unneeded build/libvapo.so
objcopy --add-gnu-debuglink=build/libvapo.debug build/libvapo.so
