# raylib-nuklear-test
add_executable(raylib-nuklear-test raylib-nuklear-test.c)
target_compile_options(raylib-nuklear-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-nuklear-test PUBLIC
    raylib
    raylib_nuklear
)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME raylib-nuklear-test COMMAND raylib-nuklear-test)
