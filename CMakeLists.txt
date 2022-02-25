cmake_minimum_required(VERSION 3.21)

option(CUIUI_BUILD_SAMPLES "Build samples" OFF)
if(CUIUI_BUILD_SAMPLES)
    list(APPEND VCPKG_MANIFEST_FEATURES "samples")
endif()

project(cuiui VERSION 0.1.0)

add_subdirectory(src)
if(CUIUI_BUILD_SAMPLES)
    include(cuiui_warnings)
    set_project_warnings(cuiui PUBLIC)
    add_subdirectory(samples)
endif()
