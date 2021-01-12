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


SceneData::SceneData() : guard((int *) 1, [](auto) {}) {
}

SceneData::~SceneData() {
    if (guard.use_count() == 1) {
        auto copy = root;
        root.clear();

        for (auto &it : copy) {
            Object::DestroyImmediate(it);
        }

        /** We don't need this asserts but to check everything is working */
        for (auto &it : components) {
            assert(it.expired());
        }

        for (auto &it : new_components) {
            assert(it.expired());
        }

        for (auto &it : loading_awake) {
            assert(it.expired());
        }
    }
}
