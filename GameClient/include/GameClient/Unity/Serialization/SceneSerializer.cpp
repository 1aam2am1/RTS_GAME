//
// Created by Michal_Marszalek on 24.11.2020.
//

#include "SceneSerializer.h"
#include <GameClient/Unity/Editor/AssetDatabase.h>

SceneSerializer::SceneSerializer() {

}

nlohmann::json SceneSerializer::operator()(const TPtr<Object> &ptr) {
    auto it = serialize_map.find(ptr);
    if (it != serialize_map.end()) {
        return it->second;
    } else {
        GUIDFileIDPack pack;

        //TODO: Prefab contains...
        if (!AssetDatabase::TryGetGUIDAndLocalFileIdentifier(ptr, pack.guid, pack.id)) {
            pack.guid = "";
            pack.id = ++max_id; //TODO: Change how we set id

            serialize_map.emplace(ptr, pack);
        }
        return pack;
    }
}

void SceneSerializer::operator()(const std::function<void(TPtr<Object>)> &ptr, const nlohmann::json &json) {
    GUIDFileIDPack pack = json.get<GUIDFileIDPack>();
    bind.emplace_back(ptr, pack);

    max_id = max_id > pack.id ? max_id : pack.id;
}

void SceneSerializer::operator()(const std::function<void(std::vector<TPtr<Object>>)> &ptr,
                                 const nlohmann::json &json) {
    bind_vector.emplace_back(ptr, json);
}
