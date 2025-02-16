#!/bin/bash
cd ..
rm /build/per
cmake -B build
cmake --build build
./build/per
