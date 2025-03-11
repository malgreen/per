#!/bin/bash

# Build in debug mode and run it
# rm /build/per
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake --build build --config Debug
if test -d build/per.app; then
    open build/per.app
elif test -f build/per; then
    ./build/per
else
    echo "binary not found"
fi


