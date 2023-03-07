#!/bin/bash

CXX=/usr/bin/g++
CXXFLAGS="-std=c++20 -Wextra -Werror -pedantic -O3"
SRC="src/extractor.cpp src/main.cpp"
LIBS=-lboost_filesystem

mkdir -p bin

echo "Building vfs_extractor"

# capture the return code of the build 
$CXX $CXXFLAGS $SRC $LIBS -o bin/vfs_extractor 
BUILD_RC=$?
 
if [ $BUILD_RC -eq 0 ]; then
    echo "Build successful"
else
    echo "Build failed"
fi