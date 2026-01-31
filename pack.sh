#!/usr/bin/env bash

if [ ! -d build ]; then
	echo "Build directory does not exist!"
	exit 1
fi

if [ ! -f build/libvapo.so ]; then
	echo "The project is not compiled!"
	exit 1
fi

if [ ! -f build/attach.sh ]; then
	echo "No attach.sh found! Wtf??"
	exit 1
fi

zip -r build/skill-issue.zip build/libvapo.so build/attach.sh lua-docs/