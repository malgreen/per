cmake_minimum_required(VERSION 3.20)

project(per)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(vendor/wxWidgets)
file(GLOB_RECURSE SOURCES "source/*.cpp")

IF(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
ELSE()
    add_executable(${PROJECT_NAME} ${SOURCES})
ENDIF()

target_link_libraries(${PROJECT_NAME} wx::core wx::base)

# target_include_directories(${PROJECT_NAME} PRIVATE vendor/wxWidgets/include)