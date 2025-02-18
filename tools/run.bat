PUSHD %~dp0
cd ..
rm \build\Debug\per.exe
cmake -B build
cmake --build build -D BUILD_SHARED_LIBS
.\build\Debug\per.exe