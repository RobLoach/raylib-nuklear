# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
        FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW 1
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# raylib-nuklear-example
add_executable(raylib-nuklear-example
    raylib-nuklear-example.c
)
target_link_libraries(raylib-nuklear-example PUBLIC
    raylib
    raylib_nuklear
)

# raylib-nuklear-demo
add_executable(raylib-nuklear-demo
    raylib-nuklear-demo.c
)
target_link_libraries(raylib-nuklear-demo PUBLIC
    raylib
    raylib_nuklear
)

# raylib-nuklear-font
add_executable(raylib-nuklear-font
    raylib-nuklear-font.c
)
target_link_libraries(raylib-nuklear-font PUBLIC
    raylib
    raylib_nuklear
)

# raylib-nuklear-font
add_executable(raylib-nuklear-font-default
    raylib-nuklear-font-default.c
)
target_link_libraries(raylib-nuklear-font-default PUBLIC
    raylib
    raylib_nuklear
)

# raylib-nuklear-texture
add_executable(raylib-nuklear-texture
    raylib-nuklear-texture.c
)
target_link_libraries(raylib-nuklear-texture PUBLIC
    raylib
    raylib_nuklear
)

# Target C99
set_property(TARGET raylib-nuklear-example PROPERTY C_STANDARD 99)
set_property(TARGET raylib-nuklear-demo PROPERTY C_STANDARD 99)
set_property(TARGET raylib-nuklear-font PROPERTY C_STANDARD 99)
set_property(TARGET raylib-nuklear-texture PROPERTY C_STANDARD 99)

# Enable warnings
if(MSVC)
  target_compile_options(raylib-nuklear-example PRIVATE /W4 /WX)
  target_compile_options(raylib-nuklear-demo PRIVATE /W4 /WX)
  target_compile_options(raylib-nuklear-font PRIVATE /W4 /WX)
  target_compile_options(raylib-nuklear-texture PRIVATE /W4 /WX)
else()
  target_compile_options(raylib-nuklear-example PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(raylib-nuklear-demo PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(raylib-nuklear-font PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(raylib-nuklear-texture PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Resources
configure_file(resources/anonymous_pro_bold.ttf resources/anonymous_pro_bold.ttf COPYONLY)
configure_file(resources/test-image.png resources/test-image.png COPYONLY)

# Web Configurations
#if (${PLATFORM} STREQUAL "Web")
if (EMSCRIPTEN)
    # Tell Emscripten to build an example.html file.
    set_target_properties(raylib-nuklear-demo PROPERTIES SUFFIX ".html")
    set_target_properties(raylib-nuklear-demo PROPERTIES OUTPUT_NAME "index")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/minshell.html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3")
    add_definitions(-DPLATFORM=Web)
    add_definitions(-DCMAKE_BUILD_TYPE=Release)
endif()
