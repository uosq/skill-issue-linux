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

if [ -f build/skill-issue.zip ]; then
	rm build/skill-issue.zip
fi

if [ ! -f build/libvapo-tracy.so ]; then
	zip -r build/skill-issue.zip build/libvapo.so build/attach.sh lua-docs/ src/
else
	if [ ! -f build/attach-tracy.sh ]; then
		echo "No attach-tracy.sh found! Did you run build-tracy.sh?"
		exit 1
	fi

	zip -r build/skill-issue.zip build/libvapo-tracy.so build/attach-tracy.sh lua-docs/ src/
fi
