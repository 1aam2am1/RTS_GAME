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
        include/GameClient/Unity/Yield/Coroutine.cpp
        include/GameClient/Unity/Yield/Coroutine.h
        include/GameClient/Unity/Physics2D/Collision2D.cpp
        include/GameClient/Unity/Physics2D/Collision2D.h
        include/GameClient/Unity/Physics2D/Joint2D.cpp
        include/GameClient/Unity/Physics2D/Joint2D.h
        include/GameClient/Unity/Physics2D/Collider2D.cpp
        include/GameClient/Unity/Physics2D/Collider2D.h
        include/GameClient/MainWindow.cpp
        include/GameClient/MainWindow.h
        include/GameClient/Unity/Editor/EditorWindow.cpp
        include/GameClient/Unity/Editor/EditorWindow.h
        include/GameClient/Unity/Core/ScriptableObject.cpp
        include/GameClient/Unity/Core/ScriptableObject.h
        include/GameClient/Unity/Macro.h
        include/GameClient/Windows/Inspector.cpp
        include/GameClient/Windows/Inspector.h
        include/GameClient/Unity/Editor/MenuCommand.h
        include/GameClient/Unity/Editor/Menu.cpp
        include/GameClient/Unity/Editor/Menu.h
        include/GameClient/Windows/About.cpp
        include/GameClient/Windows/About.h
        include/GameClient/Unity/Editor/AssetDatabase.cpp
        include/GameClient/Unity/Editor/AssetDatabase.h
        include/GameClient/Windows/AssetWindow.cpp
        include/GameClient/Windows/AssetWindow.h
        include/GameClient/WindowLayout.cpp
        include/GameClient/WindowLayout.h
        )

add_executable(GameClient ${SRC})


set_target_properties(GameClient PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_compile_definitions(GameClient PUBLIC $<$<CONFIG:DEBUG>:UNITY_EDITOR>)

target_link_libraries(GameClient sfml-network sfml-graphics)
target_link_libraries(GameClient ImGui)
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
