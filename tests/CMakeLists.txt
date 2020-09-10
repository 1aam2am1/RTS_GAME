set(SRC
        catch.hpp
        main.cpp
        $<FILTER:$<TARGET_OBJECTS:GameClient>,EXCLUDE,main.cpp.+$>
        ArgvOptions.cpp
        Signal.cpp
        TPtr.cpp
        )

add_executable(GameTests ${SRC})

set_target_properties(GameTests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_link_libraries(GameTests GameAPI)
add_flags(GameTests)

target_include_directories(GameTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../GameClient/include")
target_include_directories(GameTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../GameClient/include/GameClient/Unity")

copy_dll(GameTests GameAPI_DLL)
copy_deps_dll(GameTests)
