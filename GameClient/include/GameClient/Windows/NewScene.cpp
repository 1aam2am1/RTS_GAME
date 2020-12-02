//
// Created by Michal_Marszalek on 25.11.2020.
//

#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/SceneManagement/EditorSceneManager.h>

MENU_ITEM([]() {
    EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
}, "File/New Scene", 10)

MENU_ITEM([]() {
    newGameObject();
}, "GameObject/New GameObject", 10)

CONTEXT_MENU(GameObject, "New GameObject", []() {
    newGameObject();
})
