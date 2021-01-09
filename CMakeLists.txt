cmake_minimum_required(VERSION 3.16)
project("coel")

if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include("${CMAKE_BINARY_DIR}/conan.cmake")

conan_cmake_run(
REQUIRES
    glm/0.9.9.5
OPTIONS
    BASIC_SETUP
    CMAKE_TARGETS
BUILD
    missing
)

add_library("coel" "coel/application.cpp" "coel/window.cpp" "coel/renderer.cpp")
target_compile_features("coel" PUBLIC "cxx_std_20")
target_include_directories("coel" PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries("coel" PUBLIC CONAN_PKG::glm opengl32)

add_library("cuiui" "cuiui/user_interface.cpp" "cuiui/ui_components.cpp")
target_compile_features("cuiui" PUBLIC "cxx_std_20")
target_include_directories("cuiui" PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries("cuiui" PUBLIC CONAN_PKG::glm)

add_executable("main" "main.cpp")
target_link_libraries("main" PUBLIC "coel" "cuiui")
# target_link_options("main" PUBLIC "/ENTRY:mainCRTStartup" "/SUBSYSTEM:WINDOWS")
