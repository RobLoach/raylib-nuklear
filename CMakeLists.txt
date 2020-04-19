cmake_minimum_required(VERSION 3.11)
project(nuklear_raylib
    DESCRIPTION "nuklear_raylib: Nuklear immediate mode GUI for raylib."
    HOMEPAGE_URL "https://github.com/robloach/nuklear_raylib"
    VERSION 0.0.1
    LANGUAGES C
)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING  OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)
endif()

# nuklear
find_package(nuklear QUIET)
if (NOT nuklear_FOUND)
    add_library(nuklear INTERFACE)
    target_include_directories(nuklear INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/nuklear)
endif()

# nuklear_raylib
add_library(nuklear_raylib INTERFACE)
target_include_directories(nuklear_raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Example
option(BUILD_NUKLEAR_RAYLIB_EXAMPLES "nuklear_raylib Examples" ON)
if (BUILD_NUKLEAR_RAYLIB_EXAMPLES)
    add_executable(nuklear_raylib_example
        example/nuklear_raylib_example.c
    )
    target_link_libraries(nuklear_raylib_example PUBLIC
        raylib
        nuklear
        nuklear_raylib
    )
endif()
