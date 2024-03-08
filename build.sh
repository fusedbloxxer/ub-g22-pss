#!/usr/bin/bash

# Paths
SRC_PATH='./src'
BIN_PATH='./bin'
VCG_PATH='/opt/vcpkg/scripts/buildsystems/vcpkg.cmake'

# Options
CXX=gcc

# Build the project
cmake  "-DCMAKE_TOOLCHAIN_FILE=${VCG_PATH}" -B $BIN_PATH -S $SRC_PATH