cmake_minimum_required(VERSION 3.20)

project(Per VERSION "0.1.0")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

set(wxBUILD_SHARED ${BUILD_SHARED_LIBS})
add_subdirectory(vendor/wxWidgets)

file(GLOB_RECURSE SOURCES "source/*.cpp")

string(TIMESTAMP PROJECT_BUILD_DATE)
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE PROJECT_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
configure_file(source/utilities/about.h.in ${CMAKE_CURRENT_SOURCE_DIR}/source/utilities/about.h @ONLY)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    set(CPACK_GENERATOR "INNOSETUP")
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
    set(CPACK_GENERATOR "productbuild")
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} wx::core wx::base wx::aui)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME
    DESTINATION .
    BUNDLE
    DESTINATION .
    COMPONENT ${PROJECT_NAME}
)

# add CPack to project
set(CPACK_PACKAGE_NAME "Per")
set(CPACK_PACKAGE_VENDOR "malgreen")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Per - A lightweight HTTP GUI")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/package")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Per")
set(CPACK_COMPONENTS_ALL ${PROJECT_NAME})

# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/x.txt")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/y.txt")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/z.txt")
# set(CPACK_MONOLITHIC_INSTALL "TRUE")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # TODO: research ARM/x86 builds
    set(CPACK_SYSTEM_NAME "macOS")
endif()

include(CPack)
cpack_add_component(${PROJECT_NAME} REQUIRED)
