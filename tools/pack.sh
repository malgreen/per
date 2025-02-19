#!/bin/bash

# Build in release mode and use CPack to package it
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build --config Release
cpack --config build/CPackConfig.cmake
