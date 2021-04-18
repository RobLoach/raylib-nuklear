# raylib-nuklear-example
add_executable(raylib-nuklear-example
    raylib-nuklear-example.c
)
target_link_libraries(raylib-nuklear-example PUBLIC
    raylib
    raylib-nuklear
)

# nuklear_raylib_demo
add_executable(raylib-nuklear-demo
    raylib-nuklear-demo.c
)
target_link_libraries(raylib-nuklear-demo PUBLIC
    raylib
    raylib-nuklear
)
