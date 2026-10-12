#!/usr/bin/env bash

PID=${1:-$(pidof tf_linux64)}
PID=${PID%%[[:space:]]*}
SO="$(cd -- "$(dirname -- "$0")" && pwd)/libvapo.so"

if [ -z "$PID" ]; then
	read -r -p "tf_linux64 was not found. Enter PID: " PID
fi

if [[ ! $PID =~ ^[0-9]+$ ]]; then
	echo "Invalid PID: $PID" >&2
	exit 1
fi

gdb -q -n --batch -p "$PID" \
	-ex "set pagination off" \
	-ex "call (void*)dlopen(\"$SO\", 2)" \
	-ex "call (char*)dlerror()" \
	-ex "detach" \
	-ex "quit"
