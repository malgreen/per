#!/bin/bash

# Build in debug mode and run it
if test -d build/per.app; then
    rm -rf build/per.app
elif test -f build/per; then
    rm build/per
fi

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build -G Ninja
cmake --build build --config Debug

if test -d build/per.app; then
    open build/per.app
elif test -f build/per; then
    ./build/per
else
    echo "binary not found"
fi
