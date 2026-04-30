#!/usr/bin/env bash

PID=$(pidof -s tf_linux64)
SO=""

if [ "$MODE" = "v3" ]; then
	SO="$PWD/build/x86-64-v3/libvapo.so"
elif [ "$MODE" = "compat" ]; then
	SO="$PWD/build/x86-64/libvapo.so"
else
	if grep -q avx2 /proc/cpuinfo; then
		SO="$PWD/build/x86-64-v3/libvapo.so"
		echo "Auto: v3"
	else
		SO="$PWD/build/x86-64/libvapo.so"
		echo "Auto: compat"
	fi
fi

sudo gdb -q -n -p "$PID" \
	-ex "set pagination off" \
	-ex "set print pretty on" \
	-ex "call (void*)dlopen(\"$SO\", 2)"
