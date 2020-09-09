set(SRC
        catch.hpp
        main.cpp
        $<FILTER:$<TARGET_OBJECTS:GameClient>,EXCLUDE,main.cpp.+$>
        )

add_executable(GameTests ${SRC})

set_target_properties(GameTests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_link_libraries(GameTests GameAPI)
add_flags(GameTests)


copy_dll(GameTests GameAPI_DLL)
copy_deps_dll(GameTests)
