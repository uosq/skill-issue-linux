#!/usr/bin/env bash

PID=$(pidof tf_linux64)
SO="$PWD/build/libvapo.so"

sudo gdb -q -n -p "$PID" \
	-ex "set pagination off" \
	-ex "set print pretty on" \
	-ex "call (void*)dlopen(\"$SO\", 2)"
