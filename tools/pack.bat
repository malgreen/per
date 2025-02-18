PUSHD %~dp0
cd ..
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
sudo cmake --build build --config Release
cpack --config build/CPackConfig.cmake