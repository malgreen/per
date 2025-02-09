cmake_minimum_required(VERSION 3.16)

project(per)

set(CMAKE_C_STANDARD 99)
add_subdirectory(vendor/raylib)
file(GLOB_RECURSE SOURCES "source/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE vendor/raylib/src)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raygui/src)
target_link_libraries(${PROJECT_NAME} raylib)
