//
// Created by Michal_Marszalek on 25.11.2020.
//

#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/SceneManagement/EditorSceneManager.h>
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/Core/GameObject.h>
#include <Serialization/SceneSerializer.h>
#include <GameClient/SceneLoader.h>
#include "FileDialogWindow.h"

MENU_ITEM([]() {
    EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
}, "File/New Scene", 10)

MENU_ITEM(([]() {
    auto scene = EditorSceneManager::GetActiveScene();
    auto path = scene->path();

    if (path.empty()) {
        auto window = EditorWindow::GetWindow<FileDialogWindow>();
        window->minSize.x = 600;
        window->minSize.y = 500;
        window->ShowPopup();

        window->execute = [scene](auto path) {
            if (!path.empty()) {
                if (path.extension().string() != ".unity") {
                    path += ".unity";
                }
                EditorSceneManager::SaveScene(scene, path.generic_string(), false);
            }
        };
    } else {
        EditorSceneManager::SaveScene(EditorSceneManager::GetActiveScene(), path, false);
    }
}), "File/Save Scene", 11)

MENU_ITEM([]() {
    newGameObject();
}, "GameObject/Create empty", 10)

MENU_ITEM([]() {
    auto go = newGameObject();
    if (Selection::activeTransform) {
        go->transform()->parent = Selection::activeTransform;
    }
}, "GameObject/Create empty Child", 12)

CONTEXT_MENU(GameObject, "Create empty", []() {
    auto go = newGameObject();
    if (Selection::activeTransform) {
        go->transform()->parent = Selection::activeTransform;
    }
})

CONTEXT_MENU(GameObject, "Remove", []() {
    if (Selection::activeGameObject) {
        Object::DestroyImmediate(Selection::activeGameObject);
    }
})

CONTEXT_MENU(GameObject, "Duplicate", [](){
    if (Selection::activeGameObject) {
        SceneSerializer ser;

        auto json = ser.JsonSerializer::Serialize(Selection::activeGameObject.get());

        {
            SceneSerializer ser2;

            auto go = dynamic_pointer_cast<GameObject>(ser2.JsonSerializer::Deserialize(json));
            if(go){
                go->name += " Copy";
            }
            SceneLoader::GameObjectFix(go);
        }
    }
})
