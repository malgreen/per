cmake_minimum_required(VERSION 3.16)

project(per)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(vendor/wxWidgets)
file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE vendor/wxWidgets/include)
target_link_libraries(${PROJECT_NAME} raylib)