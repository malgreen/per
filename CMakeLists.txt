cmake_minimum_required(VERSION 3.16)

project(per)

set(CMAKE_CXX_STANDARD 14) # Are we doing C or C++?
add_subdirectory(vendor/raylib)
file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE vendor/raylib/src)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/raygui/src)
target_link_libraries(${PROJECT_NAME} raylib)
