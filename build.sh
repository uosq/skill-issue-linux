#!/usr/bin/env bash

if [ ! -d build ]; then
	mkdir build
fi

# copy our p100 attach script
cp attach.sh build/

chmod +x build/attach.sh

echo "Compiling skill issue"

# compile the cheat
make # Note: This will echo

# make .debug symbol file
objcopy --compress-debug-sections=zlib --only-keep-debug build/libvapo.so build/libvapo.debug
strip --strip-unneeded build/libvapo.so
objcopy --add-gnu-debuglink=build/libvapo.debug build/libvapo.so
