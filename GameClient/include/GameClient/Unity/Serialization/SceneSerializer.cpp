//
// Created by Michal_Marszalek on 24.11.2020.
//

#include <Macro.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/GameObjectDatabase.h>
#include "SceneSerializer.h"

std::pair<GUIDFileIDPack, bool> SceneSerializer::serialize_node_callback(TPtr<const Object> ptr) {
    GUIDFileIDPack pack;

    if (AssetDatabase::TryGetGUIDAndLocalFileIdentifier(ptr, pack.guid, pack.id)) {
        return {pack, false};
    }
    if (GameObjectDatabase::TryGetGUIDAndLocalFileIdentifier(ptr, pack.guid, pack.id)) {
        if (pack.guid == zero) {
            pack.guid = Unity::GUID{};
            return {pack, true}; // Our scene, then serialize
        } else {
            return {pack, false}; //Not our scene then reference
        }

    }
    pack.guid = Unity::GUID{};
    pack.id = Unity::GUID::NewGuid().second;

    return {pack, true};
}

TPtr<> SceneSerializer::deserialize_get_node_callback(GUIDFileIDPack pack) {
    auto assets = AssetDatabase::LoadAllAssetsAtPath(AssetDatabase::GUIDToAssetPath(pack.guid));

    AssetDatabase::fileID localID;
    Unity::GUID guid;
    for (auto a : assets) {
        if (AssetDatabase::TryGetGUIDAndLocalFileIdentifier(a, guid, localID) && localID == pack.id) {
            return a;
        }
    }
    // Get object reference from scene
    auto ob = GameObjectDatabase::TRYGetObjectFROMGUIDAndLocalFileIdentifier(!pack.guid.empty() ? pack.guid : zero,
                                                                             pack.id);
    if (ob) {
        return ob;
    }
    return TPtr<>{};
}

nlohmann::json SceneSerializer::Serialize(const std::vector<TPtr<>> &vec) {
    nlohmann::json::array_t result;
    std::vector<bool> serialize;

    result.reserve(vec.size());
    serialize.reserve(vec.size());

    for (auto &ob : vec) {
        auto it = serialize_map.find(ob);
        if (it != serialize_map.end()) {
            serialize.emplace_back(false);

        } else {
            auto[id2, s2] = serialize_node_callback(ob);
            serialize_map[ob] = id2;

            serialize.emplace_back(s2);
        }
    }

    auto ser = serialize.begin();
    for (auto ob = vec.begin(); ob != vec.end(); ++ob, ++ser) {
        result.emplace_back(InternalSerialize(ob->get(), *ser));
    }

    return result;
}

void SceneSerializer::Deserialize(std::vector<TPtr<>> &vec, const nlohmann::json &j) {
    assert(checking.expired());
    auto lock = register_check();

    vec.clear();
    vec.resize(j.size());

    if (j.is_array()) {

        int i = 0;
        for (auto &v : j.items()) {
            auto func = [&vec, i](TPtr<> n) {
                vec[i] = n;
            };
            this->operator()(func, v.value());
            ++i;
        }
    }
}
