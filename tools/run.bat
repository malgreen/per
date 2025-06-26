:: this script requires both CMake and Ninja
@echo off
PUSHD %~dp0
cd ..

del build\per.exe

cmake -DCMAKE_BUILD_TYPE=Debug -B build -G Ninja -S .
cmake --build build --config Debug

.\build\per.exe
