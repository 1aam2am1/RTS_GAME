//
// Created by Michal_Marszalek on 24.11.2020.
//

#include "SceneSerializer.h"
#include <GameClient/Unity/Editor/AssetDatabase.h>

SceneSerializer::SceneSerializer() {
    static uint64_t max_id;
    callback_id = [&](auto) -> std::pair<GUIDFileIDPack, bool> { return {{Unity::GUID("0"), ++max_id}, true}; };
}

nlohmann::json SceneSerializer::Serialize(const Object *object) {
    if (object == nullptr) { return JsonSerializer::Serialize(object); }

    GUIDFileIDPack id;

    nlohmann::json result;

    auto ob = object->shared_from_this();
    bool serialize = true;

    auto it = serialize_map.find(ob);
    if (it != serialize_map.end()) {
        return it->second;
    }

    if (callback_id) {
        auto[id2, s2] = callback_id(ob);
        serialize_map[ob] = id2;
        serialize = s2;

        id = id2;
    }

    if (serialize) {
        result = JsonSerializer::Serialize(object);
    } else {
        return id;
    }

    if (callback_id) {
        result["__Node_id"] = id;
    }

    return result;
}

void SceneSerializer::Deserialize(TPtr<Object> result, const nlohmann::json &serialized) {
    JsonSerializer::Deserialize(result, serialized);
}

void SceneSerializer::operator()(const std::function<void(TPtr<Object>)> &o, const nlohmann::json &j) {
    JsonSerializer::operator()(o, j);
}
