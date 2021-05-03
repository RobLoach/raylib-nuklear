cmake_minimum_required(VERSION 3.11)
project(raylib-nuklear
    DESCRIPTION "raylib-nuklear: Nuklear immediate mode GUI for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-nuklear"
    VERSION 1.2.0
    LANGUAGES C
)

# raylib-nuklear
add_subdirectory(include)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)
endif()

# Examples
option(BUILD_RAYLIB_NUKLEAR_EXAMPLES "Build Examples" ON)
if (BUILD_RAYLIB_NUKLEAR_EXAMPLES)
    add_subdirectory(examples)
endif()
