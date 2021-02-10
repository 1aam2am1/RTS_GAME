macro(add_versioning T)
    get_target_property(OUT ${T} LINK_LIBRARIES)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${T}.dir/version.cpp"
            #COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "${T} -> $<FILTER:$<TARGET_OBJECTS:${T}>,EXCLUDE,version.cpp.+$>"
            COMMAND ${CMAKE_COMMAND} "-DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/${T}.dir"
            -P "${PROJECT_SOURCE_DIR}/Version/CVersion.cmake"
            MAIN_DEPENDENCY "${PROJECT_SOURCE_DIR}/Version/include/Version/version.cpp.in"
            DEPENDS "$<FILTER:$<TARGET_OBJECTS:${T}>,EXCLUDE,version.cpp.+$>"
            DEPENDS "$<FILTER:${OUT},EXCLUDE,-.+$>")

    target_include_directories(${T} PUBLIC "${PROJECT_SOURCE_DIR}/Version/include")
    target_sources(${T} PUBLIC "${PROJECT_SOURCE_DIR}/Version/include/Version/version.h" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${T}.dir/version.cpp")

endmacro()
