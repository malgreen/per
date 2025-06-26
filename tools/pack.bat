:: this script requires both CMake and Ninja
@echo off
PUSHD %~dp0
cd ..
cmake -DCMAKE_BUILD_TYPE=Release -B build -G Ninja -S .
cmake --build build --config Release
cpack --config build/CPackConfig.cmake
