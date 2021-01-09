//
// Created by Michal_Marszalek on 25.11.2020.
//

#include "EditorSceneManager.h"
#include <filesystem>
#include <GameClient/Unity/Core/MonoBehaviour.h>
#include <GameClient/GlobalStaticVariables.h>
#include <Serialization/SceneSerializer.h>
#include <Macro.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/MainThread.h>

namespace fs = std::filesystem;

decltype(EditorSceneManager::dirty) EditorSceneManager::dirty;

SceneManager::SceneP EditorSceneManager::OpenScene(std::string_view scenePath, EditorSceneManager::OpenSceneMode mode) {
    /* auto meta = fs::directory_entry(scenePath);
     //TODO: Check if only filename and load path from data
     if (!meta.is_regular_file()) {
         GameApi::log(ERR.fmt("Scene %s don't exists", scenePath.data()));
         return std::shared_ptr<Scene>{new Scene(0)};
     }

     auto new_id = max_id++;
     Data result;

     result.path = scenePath;

     if (LoadSceneFull(result, scenePath)) {
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
 */
    return std::shared_ptr<Scene>{new Scene(0)};
}

SceneManager::SceneP
EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup setup, SceneManager::LoadSceneMode mode) {
    auto new_id = global.scene.max_id++;

    if (mode == LoadSceneMode::Single) {
        global.scene.data.clear(); //TODO: !! Delete root
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
            //TODO: File dialog
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
        uint64_t max_id = 0;

        serializer.callback_id = [&max_id](auto ptr) -> std::pair<GUIDFileIDPack, bool> {
            GUIDFileIDPack pack;

            if (AssetDatabase::TryGetGUIDAndLocalFileIdentifier(ptr, pack.guid, pack.id)) {
                return {pack, false};
            }
            pack.guid = "";
            pack.id = ++max_id;

            return {pack, true};
        };

        nlohmann::json result;

        for (auto &ob : global.scene.data[scene->id].root) {
            if (!ob) { continue; }

            result["objects"].emplace_back(serializer.Serialize(ob.get()));
        }

        bool ret = GameApi::saveFullFile(dstScenePath, result.dump(2, ' ', true));

        if (ret) {
            MainThread::Invoke([]() {
                AssetDatabase::Refresh(); //TODO: Delete when implemented auto refresh
            });
        }

        return ret;
    } EXCEPTION_PRINT
    return false;
}
