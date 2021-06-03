# raylib-nuklear-example
add_executable(raylib-nuklear-example
    raylib-nuklear-example.c
)
target_link_libraries(raylib-nuklear-example PUBLIC
    raylib
    raylib-nuklear
)

# raylib-nuklear-demo
add_executable(raylib-nuklear-demo
    raylib-nuklear-demo.c
)
target_link_libraries(raylib-nuklear-demo PUBLIC
    raylib
    raylib-nuklear
)

# raylib-nuklear-font
add_executable(raylib-nuklear-font
    raylib-nuklear-font.c
)
target_link_libraries(raylib-nuklear-font PUBLIC
    raylib
    raylib-nuklear
)

configure_file(resources/anonymous_pro_bold.ttf resources/anonymous_pro_bold.ttf COPYONLY)
