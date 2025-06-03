#!/bin/bash

# PCANNON CHECK.SH (utils) v1.0

if [ "$1" == "" ]; then
	cppcheck --project=build/compile_commands.json --enable=all --inconclusive --suppress=missingIncludeSystem -i vendor/

elif [ "$1" == "--help" ]; then
	echo "*** HELP ***"
	echo "[] Use cppcheck"
	echo "************"

else
	echo "Please input a valid param"
fi

