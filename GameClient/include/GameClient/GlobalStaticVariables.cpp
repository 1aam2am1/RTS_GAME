//
// Created by Michal_Marszalek on 15.12.2020.
//

#include <GameClient/Unity/Serialization/to_json.h>
#include <SceneManagement/EditorSceneManager.h>
#include "GlobalStaticVariables.h"
#include "MainThread.h"
#include "Macro.h"
#include <GameApi/BasicString.h>
#include <Core/Time.h>
#include <Editor/AssetDatabase.h>
#include <csignal>

GlobalStaticVariables global;

INITIALIZE_FUNC(MainThread::Invoke(
        []() {
            global.settings.Apply();

            {
                b2BodyDef def;
                def.position = {0, 0};
                def.angle = 0;
                def.type = b2_staticBody;

                global.physics.worldBody = global.physics.world.CreateBody(&def);
            }

            auto loaded_scene_path = AssetDatabase::GetAssetPath(EditorSceneManager::playModeStartScene.get());
            bool valid = false;
            if (!loaded_scene_path.empty()) {
                auto scene = EditorSceneManager::OpenScene(loaded_scene_path);
                valid = scene->isValid();
            }

            if (!valid) {
#if UNITY_EDITOR
                EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup::DefaultGameObjects);
#else
                GameApi::log(ERR.fmt("Start scene don't exists"));
                std::exit(-24);
#endif
            }
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

    global.physics.world.DestroyBody(global.physics.worldBody);
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
            if (!it.expired()) {
                raise(SIGINT);
            }
        }

        for (auto &it : loading_awake) {
            if (!it.expired()) {
                raise(SIGINT);
            }
        }
    }
}

uint32_t SceneData::GetID(const TPtr<Object> &o) {
    if (o.expired()) {
        return 0;
    }

    auto it = ObjectLocalID.find(o.get());
    if (it != ObjectLocalID.end()) {
        if (it->second.second)
            return it->second.first;
    }

    auto new_id = Unity::GUID::NewGuid().second;
    ObjectLocalID[o.get()] = {new_id, o};
    localIDObject[new_id] = o;
    return new_id;
}

TPtr<Object> SceneData::GetObject(uint32_t id) {
    auto it = localIDObject.find(id);
    if (it != localIDObject.end()) {
        return it->second;
    }

    return {};
}

bool SceneData::RegisterID(uint32_t s_id, const TPtr<Object> &o) {
    if (o.expired()) {
        return false;
    }

    auto it = ObjectLocalID.find(o.get());
    if (it != ObjectLocalID.end()) {
        if (it->second.second)
            return false;
    }

    auto it2 = localIDObject.find(s_id);
    if (it2 != localIDObject.end()) {
        return false;
    }

    ObjectLocalID[o.get()] = {s_id, o};
    localIDObject[s_id] = o;

    return true;
}


GlobalStaticVariables::GlobalStaticVariables() noexcept {
    physics.world.SetContactListener(&physics.listener);
    physics.world.SetAutoClearForces(true);
}

GlobalStaticVariables::~GlobalStaticVariables() {
    assert(scene.data.size() == 0);

    /// Should be null as d_add cleared all scene data, meaning all gameobjects, meaning all components
    for (auto &it : scene.components) {
        assert(it.expired());
    }

    physics.world.SetContactListener(nullptr);
    assert(physics.listener.empty()); //On Enter == On Exit
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

    if (s.contains("timeScale")) { s.at("timeScale").get_to(timeScale); }
    if (s.contains("maximumDeltaTime")) { s.at("maximumDeltaTime").get_to(maximumDeltaTime); }
    if (s.contains("fixedDeltaTime")) { s.at("fixedDeltaTime").get_to(fixedDeltaTime); }
    if (s.contains("gravity")) { s.at("gravity").get_to(gravity); }
    if (s.contains("scene")) { s.at("scene").get_to(scene_path); }
    if (s.contains("gizmo")) { s.at("gizmo").get_to(gizmo); }
    if (s.contains("orthographicSize")) { s.at("orthographicSize").get_to(old_orthographicSize); }

    if (s.contains("window")) {
        auto &w = s.at("window");
        if (w.contains("fullscreen")) { w.at("fullscreen").get_to(window.fullscreen); }
        if (w.contains("size")) { w.at("size").get_to(window.fullscreen); }
        if (w.contains("auto_size")) { w.at("auto_size").get_to(window.auto_size); }
        if (w.contains("fps")) { w.at("fps").get_to(window.fps); }
        if (w.contains("antialiasing")) { w.at("antialiasing").get_to(window.antialiasing); }
        if (w.contains("icon")) { w.at("icon").get_to(window.icon); }
    }
}

void GlobalStaticVariables::Settings::Save() {
    nlohmann::json result;

    auto &s = result["settings"];
    s["timeScale"] = timeScale;
    s["maximumDeltaTime"] = maximumDeltaTime;
    s["fixedDeltaTime"] = fixedDeltaTime;
    s["gravity"] = gravity;
    s["scene"] = scene_path;
    s["gizmo"] = gizmo;
    s["orthographicSize"] = old_orthographicSize;

    s["window"]["fullscreen"] = window.fullscreen;
    s["window"]["size"] = window.size;
    s["window"]["auto_size"] = window.auto_size;
    s["window"]["fps"] = window.fps;
    s["window"]["antialiasing"] = window.antialiasing;
    s["window"]["icon"] = window.icon;

    if (!GameApi::saveFullFile("settings.json", result.dump(2, ' ', true))) {
        GameApi::log(ERR.fmt("Can't save settings.json"));
    }
}

void GlobalStaticVariables::Settings::Apply() {
    Time::timeScale = timeScale;
    Time::maximumDeltaTime = maximumDeltaTime;
    Time::fixedDeltaTime = fixedDeltaTime;
    global.physics.world.SetGravity({gravity.x, gravity.y});
    global.mis.draw_gizmo = gizmo;
    EditorSceneManager::playModeStartScene = dynamic_pointer_cast<SceneAsset>(
            AssetDatabase::LoadMainAssetAtPath(scene_path));

    if (global.rendering.m_window) {
        sf::Image image;
        if (image.loadFromFile(global.settings.window.icon)) {
            global.rendering.m_window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
        }
    }
}
