cmake_minimum_required(VERSION 3.21)
project(cuiui VERSION 0.1.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(cuiui "src/cuiui.cpp")
add_library(cuiui::cuiui ALIAS cuiui)
target_compile_features(cuiui PUBLIC cxx_std_20)
if(BUILD_SHARED_LIBS AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(cuiui
        PRIVATE "CUIUI_EXPORT=__declspec(dllexport)"
        INTERFACE "CUIUI_EXPORT=__declspec(dllimport)")
else()
    target_compile_definitions(cuiui
        PUBLIC "CUIUI_EXPORT=__declspec(dllexport)")
endif()
target_include_directories(cuiui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

file(WRITE ${CMAKE_BINARY_DIR}/config.cmake.in [=[
@PACKAGE_INIT@
include(${CMAKE_CURRENT_LIST_DIR}/cuiui-targets.cmake)
check_required_components(cuiui)
]=])
configure_package_config_file(${CMAKE_BINARY_DIR}/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cuiui-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cuiui
    NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cuiui-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cuiui-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cuiui-config-version.cmake
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/cuiui)

install(TARGETS cuiui EXPORT cuiui-targets)
install(EXPORT cuiui-targets DESTINATION ${CMAKE_INSTALL_DATADIR}/cuiui NAMESPACE cuiui::)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/ TYPE INCLUDE)
