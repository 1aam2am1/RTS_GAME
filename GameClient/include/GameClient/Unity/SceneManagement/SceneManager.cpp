//
// Created by Michal_Marszalek on 08.09.2020.
//

#include "SceneManager.h"
#include <filesystem>
#include <GameApi/BasicString.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <GameClient/Unity/Serialization/SceneSerializer.h>
#include <GameClient/GuidFileIdPack.h>
#include <GameClient/MainThread.h>
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/SceneLoader.h>

namespace fs = std::filesystem;

void SceneManager::MoveGameObjectToScene(TPtr<GameObject> go, SceneManager::SceneP scene) {
    if (!go) {
        throw std::runtime_error("Game object should exists");//#1
    }
    {
        if (go->scene.get()) {
            //check if root
            auto t = go->transform();
            if (t->parent().get() != nullptr) {
                GameApi::log(ERR.fmt("GameObject should be root"));
                return;
            }
        }
    }

    if (!scene) {
        throw std::runtime_error("Scene should exists");//#2
    }
    if (!scene->isValid()) {
        throw std::runtime_error("Scene should be valid");//#3
    }
    if (!scene->isLoaded() &&
        global.scene.on_load_active_id.find(std::this_thread::get_id()) == global.scene.on_load_active_id.end()) {
        throw std::runtime_error("Scene should be loaded"); //&& We don't throw if this is loading proces,
    }
    auto new_scene_id = scene->id;

    if (go->scene.get() && !go->scene()->isValid()) {
        GameApi::log(ERR.fmt("Game object scene is invalid. It should not happen"));
        std::terminate(); //< Should never happen #4
    }

    auto old_scene = go->scene.get();
    if (old_scene) {
        auto &root = global.scene.data[old_scene->id].root;
        auto it = std::find_if(root.begin(), root.end(), [&](auto i) {
            return i == go;
        });
        if (it != root.end()) { root.erase(it); }
    }

    {
        auto &root = global.scene.data[new_scene_id].root;
        root.emplace_back(go);

        go->m_scene = std::shared_ptr<Scene>(new Scene(new_scene_id));
    }
    //TODO: Fix moving all components...
}

int SceneManager::sceneCount() {
    return global.scene.data.size();
}

SceneManager::SceneP SceneManager::GetActiveScene() {
    {
        auto it = global.scene.on_load_active_id.find(std::this_thread::get_id());
        if (it != global.scene.on_load_active_id.end()) {
            if (global.scene.data.find(it->second) != global.scene.data.end())
                return std::shared_ptr<Scene>(new Scene(it->second));
            return {}; //empty this is prefab!!!
        }
    }

    auto it = global.scene.data.find(global.scene.active_scene);
    if (it == global.scene.data.end()) {
        if (!global.scene.data.empty()) {
            GameApi::log(ERR.fmt("ERR active_scene isn't one of created"));
        }
        //Should only happen when there isn't any scene
        global.scene.active_scene = global.scene.max_id++;
        global.scene.data[global.scene.active_scene].isLoaded = true;
    }
    return std::shared_ptr<Scene>(new Scene(global.scene.active_scene));
}

void SceneManager::LoadScene(std::string_view sceneName, SceneManager::LoadSceneMode mode) {
    std::string scenePath{sceneName};

    MainThread::Invoke([scenePath, mode]() {
        auto new_id = global.scene.max_id++;
        {
            auto &data = global.scene.data[new_id];
            data.isLoaded = false;
            data.path = scenePath;
            data.guid = AssetDatabase::AssetPathToGUID(scenePath);
            data.name = fs::path(scenePath).stem().generic_string();
            data.buildIndex = -1;
        }

        SceneLoader::LoadSceneFull(new_id, scenePath, mode == LoadSceneMode::Single, false);
    });
}
