#!/usr/bin/bash

# Paths
SRC_PATH='./src'
BIN_PATH='./bin'
VCG_PATH='/opt/vcpkg/scripts/buildsystems/vcpkg.cmake'

# Build options
CXX=gcc

# Create the buildsystem
cmake --toolchain $VCG_PATH -G "Unix Makefiles" -B $BIN_PATH -S $SRC_PATH "$@"

# Build the project
make --directory $BIN_PATH