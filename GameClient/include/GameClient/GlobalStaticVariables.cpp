//
// Created by Michal_Marszalek on 15.12.2020.
//

#include <SceneManagement/EditorSceneManager.h>
#include "GlobalStaticVariables.h"
#include "MainThread.h"
#include "Macro.h"
#include <nlohmann/json.hpp>
#include <GameApi/BasicString.h>
#include <GameClient/Unity/Serialization/to_json.h>
#include <Core/Time.h>
#include <Editor/AssetDatabase.h>

GlobalStaticVariables global;

INITIALIZE_FUNC(MainThread::Invoke(
        []() {
            EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
            global.settings.Load();
            global.settings.Apply();
        }))

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

    assert(global.physics.world.GetBodyCount() == 0);

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

    /// Should be null as d_add cleared all scene data, meaning all gameobjects, meaning all components
    for (auto &it : scene.components) {
        assert(it.expired());
    }
}

void GlobalStaticVariables::Settings::Load() {
    auto str = GameApi::readFullFile("settings.json");
    if (str.empty()) {
        Save();
        str = GameApi::readFullFile("settings.json");
    }
    if (str.empty()) { return; }

    nlohmann::json result = nlohmann::json::parse(str);
    auto &s = result["settings"];

    s.at("timeScale").get_to(timeScale);
    s.at("maximumDeltaTime").get_to(maximumDeltaTime);
    s.at("fixedDeltaTime").get_to(fixedDeltaTime);
    s.at("gravity").get_to(gravity);
    s.at("scene").get_to(scene_path);
}

void GlobalStaticVariables::Settings::Save() {
    nlohmann::json result;

    auto &s = result["settings"];
    s["timeScale"] = timeScale;
    s["maximumDeltaTime"] = maximumDeltaTime;
    s["fixedDeltaTime"] = fixedDeltaTime;
    s["gravity"] = gravity;
    s["scene"] = scene_path;

    if (!GameApi::saveFullFile("settings.json", result.dump(2, ' ', true))) {
        GameApi::log(ERR.fmt("Can't save settings.json"));
    }
}

void GlobalStaticVariables::Settings::Apply() {
    Time::timeScale = timeScale;
    Time::maximumDeltaTime = maximumDeltaTime;
    Time::fixedDeltaTime = fixedDeltaTime;
    global.physics.world.SetGravity({gravity.x, gravity.y});
    EditorSceneManager::playModeStartScene = dynamic_pointer_cast<SceneAsset>(
            AssetDatabase::LoadMainAssetAtPath(scene_path));
}
