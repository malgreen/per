cmake_minimum_required(VERSION 3.20)

# === PROJECT SETUP ===
project(Per VERSION "0.2.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === .IN FILES SETUP === TODO
if (FALSE)
    string(TIMESTAMP PROJECT_BUILD_DATE)
    execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            OUTPUT_VARIABLE PROJECT_GIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    configure_file(
            source/utilities/about.h.in
            ${CMAKE_CURRENT_SOURCE_DIR}/source/utilities/about.h
            @ONLY
    )
endif()

# === Qt SETUP ===
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
        src/main.cpp
        src/app/main_window.cpp
        src/app/main_window.h
        src/app/main_window.ui
        src/app/tabs/http_tab.cpp
        src/app/tabs/http_tab.h
        src/app/tabs/http_tab.ui
        src/core/http.h
        src/core/http.cpp

)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Network)

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)

# === CPack SETUP === TODO
if (FALSE)
    if (WIN32)
        set(CPACK_GENERATOR "INNOSETUP")
    elseif (APPLE)
        set(CPACK_GENERATOR "productbuild")
    endif ()

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
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        # TODO: research ARM/x86 builds
        set(CPACK_SYSTEM_NAME "macOS")
    endif ()

    include(CPack)
    cpack_add_component(${PROJECT_NAME} REQUIRED)
endif ()
