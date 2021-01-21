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

    if (go->scene.get() && go->scene.get()->isValid()) {
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
}
/*
bool SceneManager::LoadSceneFull(SceneManager::Data &d, std::string_view path) {
    auto str = GameApi::readFullFile(path);
    auto asset_guid = AssetDatabase::AssetPathToGUID(path);

    try {
        auto json = nlohmann::json::parse(str.empty() ? "{}" : str);

        //d.name = (!json["settings"]["name"].empty()) ? json["settings"]["name"].get<std::string>() : "";
        d.buildIndex = -1; //TODO: Make build index

        std::unordered_map<AssetDatabase::fileID, TPtr<Object>> objects;

        SceneSerializer serializer;

        /// [{id : {GameObject: {}}}, {id : {Component: {}}}, {id : {Prefab: [GUIDFileIDPack, {GameObject: {}}]}}]
        //objects is array
        for (auto &v : json["objects"]) {
            ///asset(v.is_object());
            auto ob = v.begin(); ///< id : {GameObject: {}}

            AssetDatabase::fileID id = GameApi::to_int(ob.key());

            auto &object_type = ob.value().begin().key();
            auto &object_value = ob.value().begin().value();
            if (object_type == "Prefab") {
                //TODO: Load prefab
            } else if (object_type == "GameObject") {
                TPtr<GameObject> gm = std::shared_ptr<GameObject>(new GameObject);
                //gm->SetActive(false); //in activation set m_active false and then SetActive(v)
                ///TODO: gm->scene = SceneManager::GetActiveScene();
                serializer.Deserialize(gm, object_value);
                objects.emplace(id, gm);
            } else {
                auto obj = serializer.Deserialize(object_type, object_value);
                objects.emplace(id, obj);
            }
        }

        auto find_object = [&asset_guid, &objects](GUIDFileIDPack g) -> TPtr<Object> {
            if (g.guid == asset_guid || g.guid.empty()) {
                auto ref_obj = objects.find(g.id);
                if (ref_obj != objects.end()) {
                    return ref_obj->second;
                }
            } else {
                // Load assets from asset database
                auto assets = AssetDatabase::LoadAllAssetsAtPath(AssetDatabase::GUIDToAssetPath(g.guid));
                for (auto a : assets) {
                    AssetDatabase::fileID localID;
                    Unity::GUID guid;
                    if (AssetDatabase::TryGetGUIDAndLocalFileIdentifier(a, guid, localID) && localID == g.id) {
                        return a;
                    }
                }
            }

            return {};
        };

        for (auto &ob : serializer.bind) {
            auto found = find_object(ob.second);
            if (found) {
                ob.first(found);
            } else {
                GameApi::log(ERR.fmt("Binding: {guid: %s, fileID: %" PRIu64 "} not found.",
                                     ob.second.guid.operator std::string().data(), ob.second.id));
            }
        }
        for (auto &ob : serializer.bind_vector) {
            std::vector<TPtr<Object>> value;

            if (ob.second.is_array()) {
                value.reserve(ob.second.size());
                for (auto &v : ob.second) {
                    auto g = v.get<GUIDFileIDPack>();
                    auto found = find_object(g);

                    value.emplace_back(found);

                    if (!found && g.guid.empty() && g.id != 0) {
                        GameApi::log(ERR.fmt("Binding: {guid: %s, fileID: %" PRIu64 "} not found.",
                                             g.guid.operator std::string().data(), g.id));
                    }
                }
            }
            ob.first(value);
        }

        //root is array
        for (auto &v : json["root"]) {
            auto ob = v.get<GUIDFileIDPack>(); ///< GUIDFileIDPack
            auto it = objects.find(ob.id);
            if (it != objects.end()) {
                auto gm = dynamic_pointer_cast<GameObject>(it->second);
                if (gm) {
                    if (gm->transform()->parent().get() != nullptr) {
                        d.root.emplace_back(gm);
                    } else {
                        //It should not have parent, but we check
                        GameApi::log(ERR.fmt("Game object {guid: %s, fileID: %" PRIu64 "} "
                                             "have parent, but is root", ob.guid.operator std::string().data(),
                                             ob.id));
                    }
                }
            } else {
                GameApi::log(ERR.fmt("Root: {guid: %s, fileID: %" PRIu64 "} not found.",
                                     ob.guid.operator std::string().data(), ob.id));
            }
        }

        std::function<void(const std::vector<TPtr<Component>> &)> traverse_tree = [&](auto &vec) {

        };

        return true;
    }
    EXCEPTION_PRINT

    return false;
}
*/
/*
void SceneManager::LoadScene(std::string_view sceneName, SceneManager::LoadSceneMode mode) {
    auto meta = fs::directory_entry(sceneName);
    //TODO: Check if only filename and load path from data
    if (!meta.is_regular_file()) {
        GameApi::log(ERR.fmt("Scene %s don't exists", sceneName.data()));
        return;
    }

    std::string s;

    Data result;
    result.path = sceneName;

    auto new_id = max_id++;
    data[new_id] = result;

    MainThread::Invoke([s, mode, new_id]() {
        Data result;

        result.path = s;

        if (LoadSceneFull(result, result.path)) {
            result.isLoaded = true;

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

            data[new_id] = result;

            for (auto &g : result.objects) {
                for (auto &c : g->components) {
                    //As Component, Behaviour don't Awake we only call if MonoBehaviour
                    ///TODO: Remember that some other class should be Awakened and so on...
                    m->internalAwake();
                }
            }

        }
    });
}
*/
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
            data.name = fs::path(scenePath).stem().generic_string();
            data.buildIndex = -1;
        }

        SceneLoader::LoadSceneFull(new_id, scenePath, mode == LoadSceneMode::Single, false);
    });
}
