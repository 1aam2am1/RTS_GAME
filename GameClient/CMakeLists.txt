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
        include/GameClient/Unity/Editor/MenuCommand.h
        include/GameClient/Unity/Editor/Menu.cpp
        include/GameClient/Unity/Editor/Menu.h
        include/GameClient/Windows/About.cpp
        include/GameClient/Unity/Editor/AssetDatabase.cpp
        include/GameClient/Unity/Editor/AssetDatabase.h
        include/GameClient/Windows/AssetWindow.cpp
        include/GameClient/Windows/AssetWindow.h
        include/GameClient/WindowLayout.cpp
        include/GameClient/WindowLayout.h
        include/GameClient/IconsFontAwesome5_c.h
        include/GameClient/FontLoader.cpp
        include/GameClient/FontLoader.h
        include/GameClient/Windows/Metrics.cpp
        include/GameClient/Windows/Style.cpp
        include/GameClient/MainThread.cpp
        include/GameClient/MainThread.h
        include/GameClient/Unity/Editor/Selection.h
        include/GameClient/Unity/Editor/Selection.cpp
        include/GameClient/Unity/Core/Texture2D.cpp
        include/GameClient/Unity/Core/Texture2D.h
        include/GameClient/Unity/Core/Guid.h
        include/GameClient/Unity/Core/Guid.cpp
        include/GameClient/Unity/Core/TextAsset.cpp
        include/GameClient/Unity/Core/TextAsset.h
        include/GameClient/Unity/Editor/ImportAssetOptions.h
        include/GameClient/Unity/Editor/EditorUtility.cpp
        include/GameClient/Unity/Editor/EditorUtility.h
        include/GameClient/Unity/Editor/ObjectFactory.cpp
        include/GameClient/Unity/Editor/ObjectFactory.h
        include/GameClient/Unity/Serialization/JsonSerializer.cpp
        include/GameClient/Unity/Serialization/JsonSerializer.h
        include/GameClient/Unity/forEachMacro.h
        include/GameClient/Initializer.cpp
        include/GameClient/Initializer.h
        include/GameClient/MetaData.cpp
        include/GameClient/MetaData.h
        include/GameClient/Unity/Serialization/ISerializationCallbackReceiver.h
        include/GameClient/Unity/Editor/AssetImporter.cpp
        include/GameClient/Unity/Editor/AssetImporter.h
        include/GameClient/Importers/DirImporter.cpp
        include/GameClient/Unity/Editor/AssetImportContext.cpp
        include/GameClient/Unity/Editor/AssetImportContext.h
        include/GameClient/Importers/TextureImporter.cpp
        include/GameClient/Importers/TrueTypeFontImporter.cpp
        include/GameClient/Unity/Core/Sprite.cpp
        include/GameClient/Unity/Core/Sprite.h
        include/GameClient/Unity/Editor/OneGuidFile.h
        include/GameClient/Unity/Editor/OneGUIDFile.cpp
        include/GameClient/Windows/DemoWindow.cpp
        include/GameClient/Unity/Editor/AssetPreview.cpp
        include/GameClient/Unity/Editor/AssetPreview.h
        include/GameClient/Unity/Editor/SceneAsset.cpp
        include/GameClient/Unity/Editor/SceneAsset.h
        include/GameClient/Importers/DefaultImporter.cpp
        include/GameClient/Importers/TextScriptImporter.cpp
        include/GameClient/Importers/ShaderImporter.cpp
        include/GameClient/Unity/Editor/Editor.cpp
        include/GameClient/Unity/Editor/Editor.h
        include/GameClient/Editors/ObjectEditor.cpp
        include/GameClient/Editors/TextEditor.cpp
        include/GameClient/Editors/TextureEditor.cpp
        include/GameClient/Windows/HierarchyWindow.cpp
        include/GameClient/Unity/Core/Application.cpp
        include/GameClient/Unity/Core/Application.h
        include/GameClient/Unity/Serialization/SceneSerializer.cpp
        include/GameClient/Unity/Serialization/SceneSerializer.h
        include/GameClient/Unity/SceneManagement/EditorSceneManager.cpp
        include/GameClient/Unity/SceneManagement/EditorSceneManager.h
        include/GameClient/Windows/NewScene.cpp
        include/GameClient/Unity/Editor/ContextMenu.cpp
        include/GameClient/Unity/Editor/ContextMenu.h
        include/GameClient/Unity/Editor/MenuItem.h
        include/GameClient/GuidFileIdPack.h
        include/GameClient/Editors/GameObjectEditor.cpp
        include/GameClient/Editors/TransformEditor.cpp
        include/GameClient/Unity/Editor/DragAndDrop.cpp
        include/GameClient/Unity/Editor/DragAndDrop.h
        include/GameClient/Windows/DragDropWindow.cpp
        include/GameClient/Windows/DragDropWindow.h
        include/GameClient/imgui_custom.cpp
        include/GameClient/imgui_custom.h
        include/GameClient/Windows/SceneWindow.cpp
        include/GameClient/Windows/GameWindow.cpp
        include/GameClient/Unity/Editor/EditorApplication.cpp
        include/GameClient/Unity/Editor/EditorApplication.h
        include/GameClient/GameLoop.cpp
        include/GameClient/GameLoop.h
        include/GameClient/Unity/Core/Time.cpp
        include/GameClient/Unity/Core/Time.h
        include/GameClient/Unity/Core/Camera.cpp
        include/GameClient/Unity/Core/Camera.h
        include/GameClient/GlobalStaticVariables.cpp
        include/GameClient/GlobalStaticVariables.h
        include/GameClient/Unity/Core/Renderer.cpp
        include/GameClient/Unity/Core/Renderer.h
        include/GameClient/Unity/Core/SpriteRenderer.cpp
        include/GameClient/Unity/Core/SpriteRenderer.h
        include/GameClient/Unity/Editor/EditorGuiLayout.cpp
        include/GameClient/Unity/Editor/EditorGuiLayout.h
        include/GameClient/Windows/FileDialogWindow.cpp
        include/GameClient/Windows/FileDialogWindow.h
        include/GameClient/Unity/Serialization/to_json.h
        include/GameClient/Unity/Core/Attributes.h
        include/GameClient/Unity/Core/Attributes.cpp
        include/GameClient/Editors/TextureImporterEditor.cpp
        include/GameClient/Unity/Editor/AssetImporterEditor.cpp
        include/GameClient/Unity/Editor/AssetImporterEditor.h
        include/GameClient/Importers/TextureImporter.h
        include/GameClient/Editors/CameraEditor.cpp
        include/GameClient/Unity/Physics2D/Rigidbody2D.cpp
        include/GameClient/Unity/Physics2D/Rigidbody2D.h
        include/GameClient/Unity/Physics2D/BoxCollider2D.cpp
        include/GameClient/Unity/Physics2D/BoxCollider2D.h
        include/GameClient/Unity/Physics2D/CircleCollider2D.cpp
        include/GameClient/Unity/Physics2D/CircleCollider2D.h
        include/GameClient/Unity/Physics2D/PolygonCollider2D.cpp
        include/GameClient/Unity/Physics2D/PolygonCollider2D.h
        include/GameClient/Windows/GameSceneMenuGizmo.h
        include/GameClient/box2d/b2_user_settings.h
        include/GameClient/Unity/Physics2D/RigidbodyType2D.h
        include/GameClient/Unity/Physics2D/RigidbodyType2D.cpp
        include/GameClient/ContactListener.cpp
        include/GameClient/ContactListener.h
        include/GameClient/Components/Emitter.cpp
        include/GameClient/Windows/SceneWindow.h
        include/GameClient/Unity/Audio/AudioSource.cpp
        include/GameClient/Unity/Audio/AudioSource.h
        include/GameClient/Unity/Audio/AudioClip.cpp
        include/GameClient/Unity/Audio/AudioClip.h
        include/GameClient/Unity/Audio/AudioListener.cpp
        include/GameClient/Unity/Audio/AudioListener.h
        include/GameClient/Importers/AudioImporter.cpp
        include/GameClient/Importers/AudioImporter.h
        include/GameClient/Components/MenuPlayGame.cpp
        include/GameClient/Windows/GameSceneMenuGizmo.cpp
        include/GameClient/Components/SettingsGameWindow.cpp
        include/GameClient/SceneLoader.cpp
        include/GameClient/SceneLoader.h
        include/GameClient/Components/GenerateWorld.cpp
        include/GameClient/Components/Resource.cpp
        include/GameClient/Components/Resource.h
        include/GameClient/Components/Ship.cpp
        include/GameClient/Components/Ship.h
        include/GameClient/Components/Enemy.cpp
        include/GameClient/Components/Enemy.h
        include/GameClient/Components/mono_state.h
        include/GameClient/Components/mono_state.cpp
        include/GameClient/Components/SmartCamera.cpp
        include/GameClient/Components/UserControls.cpp
        include/GameClient/Components/UserControls.h
        include/GameClient/Components/AiEnemy.cpp
        include/GameClient/Components/AiEnemy.h
        include/GameClient/Editors/SpriteEditor.cpp
        include/GameClient/Components/ShipType.cpp
        include/GameClient/Components/ShipType.h
        include/GameClient/Unity/StateMachine/StateMachine.h
        include/GameClient/Unity/StateMachine/TransitionTo.h
        include/GameClient/Unity/StateMachine/On.h
        include/GameClient/Unity/StateMachine/OneOf.h
        include/GameClient/Unity/StateMachine/Nothing.h
        include/GameClient/Unity/StateMachine/Maybe.h
        include/GameClient/Unity/StateMachine/ByDefault.h
        include/GameClient/Components/ResourceShipStateMachine/Event.h
        include/GameClient/Components/ResourceShipStateMachine/WaitState.h
        include/GameClient/Components/ResourceShipStateMachine/AttackState.h
        include/GameClient/Components/ResourceShipStateMachine/GatheringResourceState.h
        include/GameClient/Components/ResourceShipStateMachine/TransportState.h
        include/GameClient/Components/ResourceShipStateMachine/FleeState.h
        include/GameClient/Components/ResourceShip.cpp
        include/GameClient/Components/ResourceShip.h
        include/GameClient/Components/AttackShip.cpp
        include/GameClient/Components/AttackShip.h
        include/GameClient/Components/ResourceShipStateMachine/FleeState.cpp
        include/GameClient/Unity/StateMachine.hpp
        include/GameClient/Components/ResourceShipStateMachine/WaitState.cpp
        include/GameClient/Components/ResourceShipStateMachine/TransportState.cpp
        include/GameClient/Components/ResourceShipStateMachine/GatheringResourceState.cpp
        include/GameClient/Components/ResourceShipStateMachine/AttackState.cpp
        include/GameClient/Unity/StateMachine/Will.h
        include/GameClient/Unity/Physics2D/Physics2D.cpp
        include/GameClient/Unity/Physics2D/Physics2D.h
        include/GameClient/Components/ResourceShipStateMachine/GoToState.cpp
        include/GameClient/Components/ResourceShipStateMachine/GoToState.h
        include/GameClient/Components/Building.cpp
        include/GameClient/Components/Building.h
        include/GameClient/Components/PrefabFunc.cpp
        include/GameClient/Components/PrefabFunc.h
        include/GameClient/Components/Placer.cpp
        include/GameClient/Components/Placer.h
        include/GameClient/Components/Life/Life.cpp
        include/GameClient/Components/Life/Life.h
        include/GameClient/Components/Life/Attack.cpp
        include/GameClient/Components/Life/Attack.h
        include/GameClient/Performance.cpp
        include/GameClient/Performance.h
        include/GameClient/Windows/PerformanceWindow.cpp
        include/GameClient/Components/ResourcePoint.cpp
        include/GameClient/Components/ResourcePoint.h
        include/GameClient/Components/Pause.cpp
        include/GameClient/Components/Pause.h
        include/GameClient/Components/Bullet.cpp
        include/GameClient/Components/Bullet.h
        include/GameClient/Components/AttackShipStateMachine/AttackState.cpp
        include/GameClient/Components/AttackShipStateMachine/AttackState.h
        include/GameClient/Components/AttackShipStateMachine/WaitState.cpp
        include/GameClient/Components/AttackShipStateMachine/WaitState.h
        include/GameClient/Components/AttackShipStateMachine/GoToState.cpp
        include/GameClient/Components/AttackShipStateMachine/GoToState.h
        include/GameClient/Components/AttackShipStateMachine/FleeState.cpp
        include/GameClient/Components/AttackShipStateMachine/FleeState.h
        include/GameClient/Components/AttackShipStateMachine/Attacked.cpp
        include/GameClient/Components/AttackShipStateMachine/Attacked.h
        include/GameClient/Components/AttackShipStateMachine/Patrol.cpp
        include/GameClient/Components/AttackShipStateMachine/Patrol.h
        include/GameClient/Components/Network/NetworkInterface.cpp
        include/GameClient/Components/Network/NetworkInterface.h
        include/GameClient/Components/Network/FullSynchronizer.cpp
        include/GameClient/Components/Network/FullSynchronizer.h
        include/GameClient/Components/Network/SignalSynchronizer.cpp
        include/GameClient/Components/Network/SignalSynchronizer.h
        include/GameClient/Components/Network/Synchronizer.cpp
        include/GameClient/Components/Network/Synchronizer.h
        include/GameClient/Components/TextCenter.cpp
        include/GameClient/Components/TextCenter.h
        include/GameClient/GameObjectDatabase.cpp
        include/GameClient/GameObjectDatabase.h
        include/GameClient/Components/WinLose.cpp
        include/GameClient/Components/WinLose.h
        include/GameClient/Components/ShipDummy.cpp
        include/GameClient/Components/ShipDummy.h
        )

add_executable(GameClient ${SRC})
add_executable(Editor ${SRC})


set_target_properties(GameClient PROPERTIES
        CXX_STANDARD ${CMAKE_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
set_target_properties(Editor PROPERTIES
        CXX_STANDARD ${CMAKE_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)

target_compile_definitions(Editor PUBLIC UNITY_EDITOR)
#target_compile_definitions(GameClient PUBLIC $<$<CONFIG:DEBUG>:_GLIBCXX_DEBUG>)

target_link_libraries(GameClient sfml-network sfml-graphics sfml-audio)
target_link_libraries(GameClient ImGui)
target_link_libraries(GameClient box2d)
target_link_libraries(GameClient GameAPI)
target_link_libraries(GameClient nlohmann_json)
add_versioning(GameClient)
add_flags(GameClient)

target_link_libraries(Editor sfml-network sfml-graphics sfml-audio)
target_link_libraries(Editor ImGui)
target_link_libraries(Editor box2d)
target_link_libraries(Editor GameAPI)
target_link_libraries(Editor nlohmann_json)
add_versioning(Editor)
add_flags(Editor)

target_include_directories(GameClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(GameClient PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/GameClient/Unity")

target_include_directories(Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/GameClient/Unity")

set_target_properties(GameClient PROPERTIES OUTPUT_NAME "PWR_RTS")
set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor")

if (CMAKE_BUILD_TYPE MATCHES Release)
    set_property(TARGET GameClient PROPERTY WIN32_EXECUTABLE true)
    set_property(TARGET Editor PROPERTY WIN32_EXECUTABLE true)
endif ()


copy_dll(GameClient GameAPI_DLL)
copy_deps_dll(GameClient)

copy_dll(Editor GameAPI_DLL)
copy_deps_dll(Editor)
