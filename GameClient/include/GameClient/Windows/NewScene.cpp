//
// Created by Michal_Marszalek on 25.11.2020.
//

#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/SceneManagement/EditorSceneManager.h>

void newScene() {
    EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
}

MENU_ITEM(newScene, "File/New Scene", 10)
