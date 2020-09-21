add_executable(GameViewer)


set_target_properties(GameViewer PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_link_libraries(GameViewer sfml-network sfml-graphics)
target_link_libraries(GameViewer ImGui)
target_link_libraries(GameViewer box2d)
target_link_libraries(GameViewer GameAPI)
add_versioning(GameViewer)
add_flags(GameViewer)


target_include_directories(GameViewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
        main.cpp)


target_sources(GameViewer PRIVATE ${SRC})

set_target_properties(GameViewer PROPERTIES OUTPUT_NAME "PWR_RTS Viewer")

copy_dll(GameViewer GameAPI_DLL)
copy_deps_dll(GameViewer)
