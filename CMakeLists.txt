cmake_minimum_required(VERSION 3.21)
project(cuiui VERSION 0.1.0)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
add_library(cuiui "src/platform/null/context.cpp")
add_library(cuiui::cuiui ALIAS cuiui)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(cuiui PUBLIC /std:c++latest)
else()
    target_compile_features(cuiui PUBLIC cxx_std_20)
endif()

# API selection
set(CUIUI_NATIVE_APIS NULL X11 WIN32)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CUIUI_NATIVE_API CUIUI_NATIVE_API_X11)
    list(APPEND CUIUI_AVAILABLE_NATIVE_APIS X11)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CUIUI_NATIVE_API CUIUI_NATIVE_API_WIN32)
    list(APPEND CUIUI_AVAILABLE_NATIVE_APIS WIN32)
endif()
foreach(NATIVE_API_NAME ${CUIUI_NATIVE_APIS})
    if(CUIUI_NATIVE_API STREQUAL "CUIUI_NATIVE_API_${NATIVE_API_NAME}")
        set(CUIUI_USE_${NATIVE_API_NAME} 1)
        target_compile_definitions(cuiui PUBLIC CUIUI_USE_${NATIVE_API_NAME}=1)
    else()
        set(CUIUI_USE_${NATIVE_API_NAME} 0)
        target_compile_definitions(cuiui PUBLIC CUIUI_USE_${NATIVE_API_NAME}=0)
    endif()
endforeach()
if(CUIUI_USE_X11)
    target_link_libraries(cuiui PUBLIC X11 GL ${CMAKE_DL_LIBS})
    target_sources(cuiui PRIVATE "src/platform/x11/context.cpp")
endif()
if(CUIUI_USE_WIN32)
    target_sources(cuiui PRIVATE "src/platform/win32/context.cpp")
    target_link_libraries(cuiui PRIVATE opengl32)
    target_link_libraries(cuiui PUBLIC dwmapi)
    target_compile_definitions(cuiui PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

# Build options
if(BUILD_SHARED_LIBS)
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions(cuiui
            PRIVATE "CUIUI_EXPORT=__declspec(dllexport)"
            INTERFACE "CUIUI_EXPORT=__declspec(dllimport)")
    else()
        target_compile_definitions(cuiui
            PUBLIC "CUIUI_EXPORT=")
    endif()
else()
    target_compile_definitions(cuiui
        PUBLIC "CUIUI_EXPORT=")
endif()

target_include_directories(cuiui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Packaging
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
