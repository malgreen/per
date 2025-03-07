PUSHD %~dp0
cd ..
rm \build\Debug\per.exe
cmake -B build
cmake --build build
.\build\Debug\per.exe