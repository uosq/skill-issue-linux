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