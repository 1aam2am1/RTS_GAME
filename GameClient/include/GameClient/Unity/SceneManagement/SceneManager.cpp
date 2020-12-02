//
// Created by Michal_Marszalek on 08.09.2020.
//

#include "SceneManager.h"
#include <filesystem>
#include <GameApi/BasicString.h>
#include <nlohmann/json.hpp>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <GameClient/Unity/Serialization/SceneSerializer.h>
#include <GameClient/Unity/Core/MonoBehaviour.h>
#include <GameClient/GuidFileIdPack.h>
#include <GameClient/MainThread.h>
#include <GameClient/Unity/Core/Transform.h>
#include <cinttypes>

namespace fs = std::filesystem;

uint64_t SceneManager::max_id = 1;
uint64_t SceneManager::active_scene = 0;
decltype(SceneManager::data) SceneManager::data{{0, {0, false}}};

void SceneManager::MoveGameObjectToScene(TPtr<GameObject> go, SceneManager::SceneP scene) {
    if (!go) {
        throw std::runtime_error("Game object should exists");//#1
    }
    {
        //check if root
        auto t = go->transform();
        if (t != t->root()) {
            GameApi::log(ERR.fmt("GameObject should be root"));
            return;
        }
    }

    if (!scene) {
        throw std::runtime_error("Scene should exists");//#2
    }
    if (!scene->isValid() && scene->id != active_scene) {
        throw std::runtime_error("Scene should exists");//#3
    }
    auto new_scene_id = scene->id;
    if (!scene->isValid() && scene->id == active_scene) {
        if (active_scene != 0 || (go->scene.get() && go->scene.get()->isValid())) {
            GameApi::log(ERR.fmt("Active scene is not valid and ( active_scene != 0 || go->scene->isValid())"));
            std::terminate(); //< Should never happen #4
        } else {
            //here all scenes should be invalid
            new_scene_id = max_id++; //#5
        }
    }

    auto old_scene = go->scene.get();
    if (old_scene) {
        auto it = data.find(old_scene->id);

        if (it != data.end()) {
            auto data_it = std::find_if(it->second.objects.begin(), it->second.objects.end(), [&](auto i) {
                return i == go;
            });

            it->second.objects.erase(data_it);
        }
    }

    {
        auto it = data.find(new_scene_id);

        if (it != data.end()) {
            it->second.objects.emplace_back(go);
            data[new_scene_id].isValid = true;
        } else {
            auto &new_scene = data[new_scene_id];
            new_scene.isValid = true;
            new_scene.isLoaded = true;

            new_scene.objects.emplace_back(go);
            go->m_scene = std::shared_ptr<Scene>(new Scene(new_scene_id));
        }
    }
}

bool SceneManager::LoadSceneFull(SceneManager::Data &d, std::string_view path) {
    auto str = GameApi::readFullFile(path);
    auto asset_guid = AssetDatabase::AssetPathToGUID(path);

    try {

        auto json = nlohmann::json::parse(str.empty() ? "{}" : str);

        d.name = (!json["settings"]["name"].empty()) ? json["settings"]["name"].get<std::string>() : "";
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

            return TPtr<>{nullptr};
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
                d.objects.emplace_back(it->second);
            } else {
                GameApi::log(ERR.fmt("Root: {guid: %s, fileID: %" PRIu64 "} not found.",
                                     ob.guid.operator std::string().data(), ob.id));
            }

        }

        return true;
    }
    EXCEPTION_PRINT

    return false;
}

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
            result.isValid = true;
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
                    if (auto m = dynamic_cast<MonoBehaviour *>(c.get())) { m->Awake(); }
                }
            }

        }
    });
}

int SceneManager::sceneCount() {
    if (data.size() == 0) {
        GameApi::log(ERR.fmt("sceneCount wrong count"));
        std::terminate();
    }

    return data.size() - 1;
}

SceneManager::SceneP SceneManager::GetActiveScene() {
    return SceneManager::SceneP{nullptr, std::shared_ptr<Scene>(new Scene(active_scene))};
}
