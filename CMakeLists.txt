cmake_minimum_required(VERSION 3.16)

project(per)

set(CMAKE_C_STANDARD 99)

# option(USE_VENDOR_SDL "Use vendored libraries" ON)

# if(USE_VENDOR_SDL)
# # This assumes you have added SDL as a submodule in vendored/SDL
# add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
# else()
# # 1. Look for a SDL3 package,
# # 2. look for the SDL3-shared component, and
# # 3. fail if the shared component cannot be found.
# find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
# endif()
add_subdirectory(vendor/SDL)

file(GLOB_RECURSE SOURCES "source/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

# target_include_directories(${PROJECT_NAME} PRIVATE vendor/raylib/src)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
