//
// Created by Michal_Marszalek on 15.12.2020.
//

#include <SceneManagement/EditorSceneManager.h>
#include "GlobalStaticVariables.h"
#include "MainThread.h"
#include "Macro.h"

GlobalStaticVariables global;

INITIALIZE_FUNC(MainThread::Invoke(
        []() { EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects); }))
