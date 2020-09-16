set(SRC
        include/GameClient/main.cpp
        include/GameClient/Argv_options.h
        include/GameClient/Argv_options.cpp
        include/GameClient/icon.h
        include/GameClient/icon.rc
        include/GameClient/TPtr.h
        include/GameClient/Unity/SceneManagement/SceneManager.cpp
        include/GameClient/Unity/SceneManagement/SceneManager.h
        include/GameClient/Unity/SceneManagement/Scene.cpp
        include/GameClient/Unity/SceneManagement/Scene.h
        include/GameClient/Unity/Core/MonoBehaviour.cpp
        include/GameClient/Unity/Core/MonoBehaviour.h
        include/GameClient/Unity/Yield/AsyncOperation.cpp
        include/GameClient/Unity/Yield/AsyncOperation.h
        include/GameClient/Unity/Yield/YieldInstruction.h
        include/GameClient/Unity/Core/GameObject.cpp
        include/GameClient/Unity/Core/GameObject.h
        include/GameClient/Unity/Core/Object.cpp
        include/GameClient/Unity/Core/Object.h
        include/GameClient/Unity/Core/Component.cpp
        include/GameClient/Unity/Core/Component.h
        include/GameClient/Unity/Core/Behaviour.cpp
        include/GameClient/Unity/Core/Behaviour.h
        include/GameClient/Unity/Core/Transform.cpp
        include/GameClient/Unity/Core/Transform.h
        include/GameClient/Unity/Core/Quaternion.cpp
        include/GameClient/Unity/Core/Quaternion.h
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
target_include_directories(GameClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/GameClient/Unity")


set_target_properties(GameClient PROPERTIES OUTPUT_NAME "PWR_RTS")

if (CMAKE_BUILD_TYPE MATCHES Release)
    set_property(TARGET GameClient PROPERTY WIN32_EXECUTABLE true)
endif ()


copy_dll(GameClient GameAPI_DLL)
copy_deps_dll(GameClient)
