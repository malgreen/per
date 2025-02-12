cmake_minimum_required(VERSION 3.20)

project(per)

set(CMAKE_CXX_STANDARD 20)

# SDL3 stuff
add_subdirectory(vendor/SDL)

file(GLOB_RECURSE SOURCES "source/*.cpp")

# ImGui stuff
set(IMGUI_PATH "vendor/imgui")
set(IMGUI_BACKENDS_PATH "vendor/imgui/backends")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS_PATH}/imgui_impl_sdl3.cpp
    ${IMGUI_BACKENDS_PATH}/imgui_impl_sdlgpu3.cpp
)
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH} ${IMGUI_BACKENDS_PATH})
target_link_libraries("ImGui" PRIVATE SDL3::SDL3)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL3::SDL3 ImGui)
