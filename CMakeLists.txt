cmake_minimum_required(VERSION 3.16)
project(RTS_GAME)
add_custom_target(RTS_GAME)

set(CMAKE_CXX_STANDARD 20)
link_libraries("-demangle")
if (UNIX)
    link_libraries("-ltbb")
endif ()
add_compile_options("-Wfatal-errors")
add_compile_options("$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>")
add_compile_options("$<$<CONFIG:RELEASE>:-O3;-g0;-s>")


macro(copy_dll T L)
    foreach (I IN LISTS ${L})
        get_filename_component(O "${I}" NAME)
        add_custom_command(TARGET ${T} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${I}"
                "${CMAKE_CURRENT_BINARY_DIR}/${O}"
                COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copying ${I} -> ${O}")
    endforeach ()
endmacro()


macro(copy_deps_dll T)
    add_custom_command(TARGET ${T} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -DPROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}
            -DCMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P}
            -P ${PROJECT_SOURCE_DIR}/CCopyDLL.cmake)
endmacro()


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    macro(add_flags T)
        set(FLAGS
                -Wall
                -Wextra
                -Wreturn-stack-address
                -Wattributes
                -Wdeprecated
                -Wno-unknown-pragmas)
        target_compile_options(${T} PUBLIC ${FLAGS})
    endmacro()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    macro(add_flags T)
        set(FLAGS
                -Wall
                -Wextra
                -Wreturn-local-addr
                -Wattributes
                -Wdeprecated
                -Wno-unknown-pragmas)
        target_compile_options(${T} PUBLIC ${FLAGS})
    endmacro()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    macro(add_flags T)
        set(FLAGS "")
        target_compile_options(${T} PUBLIC ${FLAGS})
    endmacro()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    macro(add_flags T)
        set(FLAGS "")
        target_compile_options(${T} PUBLIC ${FLAGS})
    endmacro()
endif ()


include(CLibsDownload.cmake)

add_subdirectory(Version)

add_subdirectory(GameApi)
add_subdirectory(ImGui)
add_subdirectory(GameClient)
add_subdirectory(tests)


add_dependencies(RTS_GAME GameClient)
add_dependencies(RTS_GAME Editor)
