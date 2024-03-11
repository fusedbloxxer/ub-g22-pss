#!/usr/bin/bash

# Paths
SRC_PATH=./src
BIN_PATH=./bin
VCG_PATH=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

# Build options
CXX=gcc

# Create the buildsystem
cmake -D CMAKE_TOOLCHAIN_FILE=$VCG_PATH -G "Unix Makefiles" -B $BIN_PATH -S $SRC_PATH "$@"

# Build the project
make --directory $BIN_PATH