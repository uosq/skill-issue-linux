#!/usr/bin/env bash

# has avx2
if grep -q avx2 /proc/cpuinfo; then
	echo "x86-64-v3"
else
	echo "x86-64"
fi