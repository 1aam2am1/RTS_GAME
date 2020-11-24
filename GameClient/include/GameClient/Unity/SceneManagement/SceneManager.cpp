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

namespace fs = std::filesystem;

uint64_t SceneManager::max_id = 1;
decltype(SceneManager::data) SceneManager::data{{0, {0, false}}};

void SceneManager::MoveGameObjectToScene(TPtr<GameObject> go, SceneManager::SceneP scene) {
    if (!go || !scene || !scene->isValid()) {
        throw std::runtime_error("Scene and game object should exists");
    }
    auto existing_id = go->scene.get()->id;
    auto goto_id = scene->id;

    auto it = data.find(existing_id);
    auto goto_it = data.find(goto_id);

    if (it != data.end() && goto_it != data.end()) {

        //if is root
        auto data_it = std::find_if(it->second.objects.begin(), it->second.objects.end(), [&](auto i) {
            return i == go;
        });

        if (data_it != it->second.objects.end()) {
            goto_it->second.objects.emplace_back(go);
            go->m_scene = std::shared_ptr<Scene>(new Scene(goto_id));
        }
    }
}

bool SceneManager::LoadSceneFull(SceneManager::Data &d, std::string_view path) {
    auto str = GameApi::readFullFile(path);
    auto asset_guid = AssetDatabase::AssetPathToGUID(path);

    auto json = nlohmann::json::parse(str.empty() ? "{}" : str);

    d.name = (!json["settings"]["name"].empty()) ? json["settings"]["name"].get<std::string>() : "";
    d.buildIndex = -1; //TODO: Make build index


    try {
        std::unordered_map<TPtr<Object> *, nlohmann::json> bindings;
        std::unordered_map<AssetDatabase::fileID, TPtr<Object>> objects;

        SceneSerializer serializer(bindings);

        for (auto &v : json["GameObject"].items()) {
            AssetDatabase::fileID id = GameApi::to_int(v.key());

            auto obj = dynamic_pointer_cast<GameObject>(serializer.Deserialize(typeid(GameObject), v.value()));

            objects.emplace(id, obj);
            d.objects.push_back(obj);
        }
        for (auto &v : json["Component"].items()) {
            AssetDatabase::fileID id = GameApi::to_int(v.key());

            auto p = v.value().get<std::pair<std::string, nlohmann::json>>();
            auto obj = serializer.Deserialize(p.first, p.second);

            objects.emplace(id, obj);
        }
        for (auto &ob : bindings) {
            auto guid = ob.second["guid"].get<Unity::GUID>();
            AssetDatabase::fileID id;
            ob.second["fileID"].get_to(id);

            if (guid == asset_guid || guid.empty()) {
                auto ref_obj = objects.find(id);
                if (ref_obj != objects.end()) {
                    *ob.first = ref_obj->second;
                }
            } else {
                // Load assets from asset database
                auto assets = AssetDatabase::LoadAllAssetsAtPath(AssetDatabase::GUIDToAssetPath(guid));
                for (auto a : assets) {
                    AssetDatabase::fileID localID;
                    if (AssetDatabase::TryGetGUIDAndLocalFileIdentifier(a, guid, localID) && localID == id) {
                        *ob.first = a;
                        break;
                    }
                }
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

    auto &d = data[max_id++];
    d.path = sceneName;

    if (LoadSceneFull(d, sceneName)) {
        d.isValid = true;
        d.isLoaded = true;

        if (mode == LoadSceneMode::Single) {
            auto copy = d;
            data.clear();
            data[max_id - 1] = d;
        }

    } else {
        d.isValid = false;
        d.isLoaded = true;
    }

    for (auto &g : d.objects) {
        for (auto &c : g->components) {
            //As Component, Behaviour don't Awake we only call if MonoBehaviour
            ///TODO: Remember that some other class should be Awakened and so on...
            if (auto m = dynamic_cast<MonoBehaviour *>(c.get())) { m->Awake(); }
        }
    }
}

