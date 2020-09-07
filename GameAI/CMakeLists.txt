add_executable(GameAI)


set_target_properties(GameAI PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_link_libraries(GameAI sfml-network sfml-graphics)
target_link_libraries(GameAI tgui)
target_link_libraries(GameAI box2d)
target_link_libraries(GameAI GameAPI)
add_versioning(GameAI)
add_flags(GameAI)


target_include_directories(GameAI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
        main.cpp)

target_sources(GameAI PRIVATE ${SRC})

copy_dll(GameAI GameAPI_DLL)
copy_deps_dll(GameAI)
