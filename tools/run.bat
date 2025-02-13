PUSHD %~dp0
cd ..
cmake -B build
cmake --build build
.\build\Debug\per.exe