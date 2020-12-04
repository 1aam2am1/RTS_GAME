//
// Created by Michal_Marszalek on 25.11.2020.
//

#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/SceneManagement/EditorSceneManager.h>
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/Unity/Core/Transform.h>

MENU_ITEM([]() {
    EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
}, "File/New Scene", 10)

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
