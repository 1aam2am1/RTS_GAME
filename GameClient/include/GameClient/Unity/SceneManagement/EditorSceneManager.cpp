//
// Created by Michal_Marszalek on 25.11.2020.
//

#include "EditorSceneManager.h"
#include <filesystem>
#include <GameClient/GlobalStaticVariables.h>
#include <Serialization/SceneSerializer.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/MainThread.h>
#include <GameClient/Windows/FileDialogWindow.h>
#include <GameClient/Unity/Core/Transform.h>
#include <Core/Attributes.h>
#include <Editor/EditorApplication.h>
#include <GameClient/SceneLoader.h>

namespace fs = std::filesystem;

static std::unordered_set<uint64_t> dirty;
decltype(EditorSceneManager::playModeStartScene) EditorSceneManager::playModeStartScene;

SceneManager::SceneP EditorSceneManager::OpenScene(std::string_view scenePath, EditorSceneManager::OpenSceneMode mode) {
    {
        auto scene = dynamic_pointer_cast<SceneAsset>(AssetDatabase::LoadMainAssetAtPath(std::string{scenePath}));
        if (!scene) {
            GameApi::log(ERR.fmt("Scene %s don't exists", scenePath.data()));
            return std::shared_ptr<Scene>{new Scene(0)};
        }
    }

    auto new_id = global.scene.max_id++;
    {
        auto &data = global.scene.data[new_id];
        data.isLoaded = false;
        data.path = scenePath;
        data.name = fs::path(scenePath).stem().generic_string();
        data.buildIndex = -1;
    }

    SceneLoader::LoadSceneFull(new_id, scenePath, mode == OpenSceneMode::Single, true);

    return std::shared_ptr<Scene>{new Scene(new_id)};
}

SceneManager::SceneP
EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup setup, SceneManager::LoadSceneMode mode) {
    auto new_id = global.scene.max_id++;

    if (mode == LoadSceneMode::Single) {
        auto old = std::move(global.scene.data);
        global.scene.data.clear();
        MainThread::Invoke([old]() {}); //< Here delete old data

        global.scene.active_scene = new_id;
    }

    global.scene.data[new_id].isLoaded = true;

    if (setup == NewSceneSetup::DefaultGameObjects) {
        auto ob = newGameObject<Camera>("MainCamera");
        ob->tag = "MainCamera";
    }

    return std::shared_ptr<Scene>{new Scene(new_id)};
}

bool EditorSceneManager::MarkSceneDirty(SceneManager::SceneP scene) {
    auto it = global.scene.data.find(scene->id);
    if (it != global.scene.data.end()) {
        dirty.emplace(it->first);
        return true;
    }
    return false;
}

bool EditorSceneManager::SaveScene(SceneManager::SceneP scene, std::string_view dstScenePath, bool saveAsCopy) {
    fs::path p;
    if (!scene || !scene->isValid() || !scene->isLoaded()) { return false; }

    if (dstScenePath.empty()) {
        if (global.scene.data[scene->id].path.empty()) {
            auto window = EditorWindow::GetWindow<FileDialogWindow>();
            window->minSize.x = 600;
            window->minSize.y = 500;
            window->ShowPopup();

            window->execute = [scene, saveAsCopy](auto path) {
                if (!path.empty())
                    SaveScene(scene, path.generic_string(), saveAsCopy);
            };

            return false;
        } else {
            p = global.scene.data[scene->id].path;
        }

    } else {
        p = dstScenePath;
    }

    if (p.generic_string().substr(0, 7) != "Assets/") { return false; }

    try {
        SceneSerializer serializer;

        nlohmann::json result;

        {
            std::vector<TPtr<Object>> c{};
            c.reserve(global.scene.data[scene->id].root.size());
            std::copy(global.scene.data[scene->id].root.begin(), global.scene.data[scene->id].root.end(),
                      back_inserter(c));
            result["objects"] = serializer.Serialize(c);
        }

        bool ret = GameApi::saveFullFile(p.generic_string(), result.dump(2, ' ', true));

        if (ret) {
            MainThread::Invoke([]() {
                AssetDatabase::Refresh(); //TODO: Delete this when implemented auto refresh
            });
            if (!saveAsCopy) {
                if (global.scene.data[scene->id].path.empty()) {
                    global.scene.data[scene->id].path = std::string{dstScenePath};
                    global.scene.data[scene->id].name = p.stem().generic_string();
                }
                dirty.erase(scene->id);
            }
        }

        return ret;
    } EXCEPTION_PRINT
    return false;
}
