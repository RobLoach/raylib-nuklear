# nuklear_raylib
add_library(nuklear_raylib INTERFACE)
target_include_directories(nuklear_raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES
    nuklear_raylib.h
    nuklear_raylib_nuklear.h
    DESTINATION include
)
