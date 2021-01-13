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

static int UNIQUE_ID(p) = Initializer::d_add([]() {
    global.scene.data.clear();

    global.assets.path_files.clear();
    global.assets.dir_tree.clear();

    for (auto iterator: global.assets.objects) {
        std::for_each(std::execution::par_unseq, iterator.second.object.begin(), iterator.second.object.end(),
                      [](auto &&f) {
                          Object::DestroyImmediate(f.second, true);
                      });
    }
});

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
