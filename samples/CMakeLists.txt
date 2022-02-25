project(${PROJECT_NAME}_samples)

function(add_sample)
    set(F_ARGS CONSOLE_APP)
    set(S_ARGS )
    set(M_ARGS LIBS FOLDER)
    cmake_parse_arguments(ARGS "${F_ARGS}" "${S_ARGS}" "${M_ARGS}" ${ARGN})
    set(FOLDER_PATH)
    set(NAME_PREFIX)
    if(ARGS_FOLDER)
        foreach(SUBFOLDER ${ARGS_FOLDER})
            set(FOLDER_PATH "${FOLDER_PATH}${SUBFOLDER}/")
            set(NAME_PREFIX "${NAME_PREFIX}_${SUBFOLDER}")
        endforeach()
    endif()
    set(TGT ${PROJECT_NAME}${NAME_PREFIX})
    set(FOLDER_PATH "${FOLDER_PATH}")
    add_executable(${TGT} "${FOLDER_PATH}/main.cpp")
    if(ARGS_LIBS)
        target_link_libraries(${TGT} PRIVATE ${ARGS_LIBS})
    endif()
    target_include_directories(${TGT} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
    if(NOT ARGS_CONSOLE_APP)
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_link_options(${TGT} PUBLIC -Wl,/SUBSYSTEM:WINDOWS,/ENTRY:mainCRTStartup)
            elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
                target_link_options(${TGT} PUBLIC /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
            endif()
        endif()
    endif()
endfunction()

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_sample(FOLDER 1_getting_started 1_basic
    CONSOLE_APP
    LIBS
        glfw
        glad::glad
        cuiui::cuiui
)
