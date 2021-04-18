# raylib-nuklear
add_library(raylib-nuklear INTERFACE)
target_include_directories(raylib-nuklear INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES
    raylib-nuklear.h
    raylib-nuklear-nuklear.h
    DESTINATION include
)
