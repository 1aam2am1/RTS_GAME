//
// Created by Michal_Marszalek on 25.11.2020.
//

#include "EditorSceneManager.h"
#include <filesystem>
#include <GameClient/Unity/Core/MonoBehaviour.h>

namespace fs = std::filesystem;

decltype(EditorSceneManager::dirty) EditorSceneManager::dirty;

SceneManager::SceneP EditorSceneManager::OpenScene(std::string_view scenePath, EditorSceneManager::OpenSceneMode mode) {
    auto meta = fs::directory_entry(scenePath);
    //TODO: Check if only filename and load path from data
    if (!meta.is_regular_file()) {
        GameApi::log(ERR.fmt("Scene %s don't exists", scenePath.data()));
        return SceneManager::SceneP{nullptr, std::shared_ptr<Scene>(new Scene(0))};
    }

    auto new_id = max_id++;
    Data result;

    result.path = scenePath;

    if (LoadSceneFull(result, scenePath)) {
        result.isValid = true;
        result.isLoaded = true;

        switch (mode) {
            case OpenSceneMode::Single: {
                data.clear();
                data[new_id] = result;
                active_scene = new_id;
                break;
            }
            case OpenSceneMode::Addictive:
                data[new_id] = result;
                break;
        }

    } else {
        result.isValid = false;
        result.isLoaded = true;
    }

    return SceneManager::SceneP{nullptr, std::shared_ptr<Scene>(new Scene(new_id))};
}

SceneManager::SceneP
EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup setup, SceneManager::LoadSceneMode mode) {
    auto new_id = max_id++;
    Data result;

    result.isLoaded = true;
    result.isValid = true;
    result.name = "Scene";

    switch (setup) {
        case NewSceneSetup::DefaultGameObjects:
            //TODO: Add default objects: camera...
        case NewSceneSetup::EmptyScene:
            break;
    }

    switch (mode) {
        case LoadSceneMode::Single: {
            data.clear();
            data[new_id] = result;
            active_scene = new_id;
            break;
        }
        case LoadSceneMode::Addictive:
            data[new_id] = result;
            break;
    }

    return SceneManager::SceneP{nullptr, std::shared_ptr<Scene>(new Scene(new_id))};
}

bool EditorSceneManager::MarkSceneDirty(SceneManager::SceneP scene) {
    auto it = data.find(scene->id);
    if (it != data.end()) {
        dirty.emplace(it->first);
        return true;
    }
    return false;
}
