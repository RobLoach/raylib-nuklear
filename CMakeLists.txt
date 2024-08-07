cmake_minimum_required(VERSION 3.11)
project(raylib_nuklear
    DESCRIPTION "raylib_nuklear: Nuklear immediate mode GUI for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-nuklear"
    VERSION 5.0.1
    LANGUAGES C
)

# raylib-nuklear
add_subdirectory(include)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYLIB_NUKLEAR_IS_MAIN TRUE)
else()
    set(RAYLIB_NUKLEAR_IS_MAIN FALSE)
endif()
option(RAYLIB_NUKLEAR_BUILD_EXAMPLES "Examples" ${RAYLIB_NUKLEAR_IS_MAIN})

# Examples
if (RAYLIB_NUKLEAR_BUILD_EXAMPLES)
    add_subdirectory(examples)

    # Testing
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        # set(CTEST_CUSTOM_TESTS_IGNORE
        #     pkg-config--static
        # )
        # Always print verbose output when tests fail if run using `make test`.
        list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
        add_subdirectory(test)
    endif()
endif()
