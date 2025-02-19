#!/bin/bash

# Build in debug mode and run it
# rm /build/per
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake --build build --config Debug
# ./build/per
