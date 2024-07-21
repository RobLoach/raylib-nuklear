# raylib_nuklear
add_library(raylib_nuklear INTERFACE)
target_include_directories(raylib_nuklear INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES
    raylib-nuklear.h
    raylib-nuklear-font.h
    nuklear.h
    DESTINATION include
)
