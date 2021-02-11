//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "FullSynchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Serialization/SceneSerializer.h>
#include <Core/Attributes.h>
#include "GameClient/Components/Ship.h"

ADD_USER_COMPONENT(FullSynchronizer, send)

void FullSynchronizer::IntUpdate() {
    if (send) {
        SendStatus();
    }
}

void FullSynchronizer::ReceiveMessage(const nlohmann::json &json) {
    if (json.contains("__GO")) { json.at("__GO").get_to(gameObject()->name); }
    if (json.contains("__Ly")) { json.at("__Ly").get_to(gameObject()->layer); }

    if (!json.contains("__Node_id") || !json.at("__Node_id").is_object()) {
        GameApi::log(ERR << "Wrong json message: " << json.dump(2, ' '));
        return;
    }
    auto id = json.at("__Node_id").get<GUIDFileIDPack>();

    auto o = GetObject(id);

    SceneSerializer serializer;
    for (auto &ob : json.items()) {
        if (ob.key().starts_with("__")) { continue; }

        if (!o) {
            if (ob.key() == "Transform") {
                o = transform();
            } else {
                o = gameObject()->AddComponent(
                        MetaData::getReflection(ob.key()).type); //Create new game object as it don't have id
            }

            if (!o) {
                GameApi::log(ERR << "Can't add new component");
            }
            RegisterID(id.id, o);
        }

        ///TODO: Fix serializers because there are some mayor errors in working with them
        serializer.Update(o, ob.value());

        break;
    }

    if (send) {
        GameApi::log(ERR << "We have received message even when we are sending data not receiving");
    }
}

bool FullSynchronizer::SendStatus() {
    auto vec = GetComponents<Component>();
    bool result = true;

    for (auto &v : vec) {
        if (v.get() == this) [[unlikely]] { continue; }
        if (Attributes::CheckCustomAttribute(v, DontNetworkSynchronize))[[unlikely]] { continue; }

        SceneSerializer serializer;

        auto json = serializer.JsonSerializer::Serialize(v.get());

        json["__Ly"] = gameObject()->layer;
        json["__GO"] = gameObject()->name;
        result = result && SendMessage(json);
    }

    return result;
}


