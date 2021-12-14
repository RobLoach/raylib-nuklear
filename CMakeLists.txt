cmake_minimum_required(VERSION 3.11)
project(raylib-nuklear
    DESCRIPTION "raylib-nuklear: Nuklear immediate mode GUI for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-nuklear"
    VERSION 4.0.1
    LANGUAGES C
)

# raylib-nuklear
add_subdirectory(include)

# Examples
option(BUILD_RAYLIB_NUKLEAR_EXAMPLES "Build Examples" ON)
if (BUILD_RAYLIB_NUKLEAR_EXAMPLES)
    # raylib
    find_package(raylib QUIET)
    if (NOT raylib_FOUND)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib)
    endif()
    add_subdirectory(examples)
endif()

# Testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    enable_testing()
    if(BUILD_TESTING AND BUILD_RAYLIB_NUKLEAR_EXAMPLES)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
        add_subdirectory(test)
    endif()
endif()
