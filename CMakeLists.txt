cmake_minimum_required(VERSION 3.11)
project(nuklear_raylib
    DESCRIPTION "nuklear_raylib: Nuklear immediate mode GUI for raylib."
    HOMEPAGE_URL "https://github.com/robloach/nuklear_raylib"
    VERSION 0.1.0
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

add_subdirectory(include)

# Example
option(BUILD_NUKLEAR_RAYLIB_EXAMPLES "nuklear_raylib Examples" ON)
if (BUILD_NUKLEAR_RAYLIB_EXAMPLES)
    add_subdirectory(examples)
endif()
