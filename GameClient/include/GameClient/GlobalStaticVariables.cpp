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

static int UNIQUE_ID(p) = Initializer::d_add([]() { global.scene.data.clear(); });

SceneData::SceneData() : guard((int *) 1, [](auto) {}) {
}

SceneData::~SceneData() {
    if (guard.use_count() == 1) {
        for (auto &it : root) {
            Object::DestroyImmediate(it);
        }

        /** We don't need this asserts but to check everything is working */
        for (auto &it : new_components) {
            assert(it.expired());
        }

        for (auto &it : loading_awake) {
            assert(it.expired());
        }
    }
}

GlobalStaticVariables::~GlobalStaticVariables() {
    assert(scene.data.size() == 0);

    /// Schould be null as d_add cleared all scene data, meaning all gameobjects, meaning all components
    for (auto &it : scene.components) {
        assert(it.expired());
    }
}
