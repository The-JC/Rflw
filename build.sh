#!/bin/bash

# select to clean previous builds
: ${CLEANBUILD:=false}

if [ "${CLEANBUILD}" == "true" ]; then
    echo "- removing build directory!"
    rm -rf build
fi
mkdir build
cd build

cmake -D "CMAKE_TOOLCHAIN_FILE=../CMake/GNU-ARM-Toolchain.cmake" ../
echo "$1"
if [ "$1" == "flash" ]; then
	echo "Building project and flashing to ST-Link"
	make flash
elif [ "$1" == "erase" ]; then
	echo "Building project and flashing to ST-Link"
	make erase
elif [ "$1" == "debug" ]; then
	echo "Debug project and flashing to ST-Link"
	make debug-server
elif [ "$1" == "" ]; then
	echo "Building project"
	make
else
	echo "Unrecognized parameter"
fi