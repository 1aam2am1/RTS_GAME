add_executable(GameServer)


set_target_properties(GameServer PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_link_libraries(GameServer sfml-network)
target_link_libraries(GameServer box2d)
target_link_libraries(GameServer GameAPI)
add_versioning(GameServer)
add_flags(GameServer)


target_include_directories(GameServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
        main.cpp)


target_sources(GameServer PRIVATE ${SRC})

set_target_properties(GameServer PROPERTIES OUTPUT_NAME "PWR_RTS SERVER")


copy_dll(GameServer GameAPI_DLL)
copy_deps_dll(GameServer)
