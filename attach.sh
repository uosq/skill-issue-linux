#!/usr/bin/env bash

PID=$(pidof -s tf_linux64)
MODE="$1"
SO=""

if [ "$MODE" = "v3" ]; then
	SO="$PWD/x86-64-v3/libvapo.so"
elif [ "$MODE" = "compat" ]; then
	SO="$PWD/x86-64/libvapo.so"
else
	if grep -q avx2 /proc/cpuinfo; then
		SO="$PWD/x86-64-v3/libvapo.so"
		echo "Auto: v3"
	else
		SO="$PWD/x86-64/libvapo.so"
		echo "Auto: compat"
	fi
fi

gdb -q -n --batch -p "$PID" \
	-ex "set pagination off" \
	-ex "call (void*)dlopen(\"$SO\", 2)" \
	-ex "call (char*)dlerror()" \
	-ex "detach" \
	-ex "quit"
