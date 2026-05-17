include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# raylib_nuklear
add_library(raylib_nuklear INTERFACE)
add_library(raylib-nuklear::raylib-nuklear ALIAS raylib_nuklear)

target_include_directories(raylib_nuklear INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(FILES
    raylib-nuklear.h
    nuklear.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS raylib_nuklear
    EXPORT raylib-nuklearTargets
)

install(EXPORT raylib-nuklearTargets
    FILE raylib-nuklearTargets.cmake
    NAMESPACE raylib-nuklear::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-nuklear
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/raylib-nuklearConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/raylib-nuklearConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-nuklear
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/raylib-nuklearConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/raylib-nuklearConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/raylib-nuklearConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raylib-nuklear
)
