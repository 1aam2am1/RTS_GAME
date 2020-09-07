set(SRC
        include/GameClient/main.cpp
        include/GameClient/Argv_options.h
        include/GameClient/Argv_options.cpp
        include/GameClient/icona.h
        include/GameClient/icona.rc
        )

add_executable(GameClient ${SRC})


set_target_properties(GameClient PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)


target_link_libraries(GameClient sfml-network sfml-graphics)
target_link_libraries(GameClient tgui)
target_link_libraries(GameClient box2d)
target_link_libraries(GameClient GameAPI)
add_versioning(GameClient)
add_flags(GameClient)

target_include_directories(GameClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")


set_target_properties(GameClient PROPERTIES OUTPUT_NAME "PWR_RTS")

if (CMAKE_BUILD_TYPE MATCHES Release)
    set_property(TARGET GameClient PROPERTY WIN32_EXECUTABLE true)
endif ()


copy_dll(GameClient GameAPI_DLL)
copy_deps_dll(GameClient)
