PUSHD %~dp0
cd ..
dir /b /s /a "vendor/SDL" | findstr .>nul || dir /b /s /a "vendor/imgui" | findstr .>nul || (
    echo Remember to initialize submodules
    exit
)
cmake -B build
cmake --build build
.\build\Debug\per.exe