# nuklear_raylib_example
add_executable(nuklear_raylib_example
    nuklear_raylib_example.c
)
target_link_libraries(nuklear_raylib_example PUBLIC
    raylib
    nuklear_raylib
)

# nuklear_raylib_demo
add_executable(nuklear_raylib_demo
    nuklear_raylib_demo.c
)
target_link_libraries(nuklear_raylib_demo PUBLIC
    raylib
    nuklear_raylib
)
