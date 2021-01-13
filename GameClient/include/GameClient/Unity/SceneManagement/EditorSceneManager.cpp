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

namespace fs = std::filesystem;

decltype(EditorSceneManager::dirty) EditorSceneManager::dirty;

SceneManager::SceneP EditorSceneManager::OpenScene(std::string_view scenePath, EditorSceneManager::OpenSceneMode mode) {
    {
        auto scene = AssetDatabase::LoadMainAssetAtPath(std::string{scenePath});
        if (!dynamic_pointer_cast<SceneAsset>(scene)) {
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

    {
        global.scene.on_load_active_id[std::this_thread::get_id()] = new_id;
        std::shared_ptr<int> lock((int *) 1,
                                  [](auto) { global.scene.on_load_active_id.erase(std::this_thread::get_id()); });

        try {
            auto str = GameApi::readFullFile(scenePath);
            auto json = nlohmann::json::parse(str);

            {
                SceneSerializer serializer;
                std::vector<TPtr<>> result;

                serializer.Deserialize(result, json["objects"]);
            }
            //creation of gameobjects adds them to the scene

            std::function<void(TPtr<GameObject>)> fix = [&fix](TPtr<GameObject> go) {
                if (!go) { return; }

                decltype(go->components) copy = std::move(go->components);
                go->components.clear();

                for (auto &c : copy) {
                    go->AddComponent(c);
                }

                for (int i = 0; i < go->transform()->childCount(); ++i) {
                    auto child = go->transform()->GetChild(i);
                    fix(child);
                }
            };

            for (auto &ob : global.scene.data[new_id].root) {
                fix(ob);
            }

            global.scene.data[new_id].isLoaded = true;
            for (auto &ob : global.scene.data[new_id].loading_awake) {
                //activeInHierarchy => true
                assert(ob->gameObject()->activeInHierarchy());
                // Should not happen, maybe if we stopped playing when there was active loading,
                // therefore it should happen between frames
                // pause=>do, play-not=>only execute in edit mode
                //if (Application::isPlaying() || Attributes::CheckCustomAttribute(ob, ExecuteInEditMode)) {
                ob->UnityAwake();
                //}
            }
            global.scene.data[new_id].loading_awake.clear();

        } catch (const std::exception &e) {
            global.scene.data.erase(new_id);
            GameApi::log(ERR.fmt("%s", e.what()));
            return std::shared_ptr<Scene>{new Scene(0)};
        }
    }

    if (mode == OpenSceneMode::Single) {
        auto copy = std::move(global.scene.data[new_id]);
        global.scene.data.clear();
        global.scene.data[new_id] = copy;

        global.scene.active_scene = new_id;
    }

    return std::shared_ptr<Scene>{new Scene(new_id)};
}

SceneManager::SceneP
EditorSceneManager::NewScene(EditorSceneManager::NewSceneSetup setup, SceneManager::LoadSceneMode mode) {
    auto new_id = global.scene.max_id++;

    if (mode == LoadSceneMode::Single) {
        global.scene.data.clear();
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

        bool ret = GameApi::saveFullFile(dstScenePath, result.dump(2, ' ', true));

        if (ret) {
            MainThread::Invoke([]() {
                AssetDatabase::Refresh(); //TODO: Delete this when implemented auto refresh
            });
            if (!saveAsCopy) {
                dirty.erase(scene->id);
            }
        }

        return ret;
    } EXCEPTION_PRINT
    return false;
}
