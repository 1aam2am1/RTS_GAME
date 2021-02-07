//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "FullSynchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Serialization/SceneSerializer.h>

ADD_USER_COMPONENT(FullSynchronizer, send)

void FullSynchronizer::Update() {
    if (send) {
        auto vec = GetComponents<Component>();

        for (auto &v : vec) {
            if (v.get() == this) { continue; }

            SceneSerializer serializer;

            auto json = serializer.JsonSerializer::Serialize(v.get());

            json["__FullSynchronizerID"] = GetID(v);
            json["__Ly"] = gameObject()->layer;
            SendMessage(json);
        }
    }
}

void FullSynchronizer::ReceiveMessage(const nlohmann::json &json) {
    if (!send) {
        auto id = json["__FullSynchronizerID"].get<uint32_t>();
        gameObject()->layer = json["__Ly"].get<int>(); //Change this, when fixed serialization

        auto o = GetObject(id);

        SceneSerializer serializer;
        for (auto &ob : json.items()) {
            if (ob.key().starts_with("__")) { continue; }

            if (!o) {
                o = gameObject()->AddComponent(
                        MetaData::getReflection(ob.key()).type); //Create new game object as it don't have id
                RegisterID(id, o);
            }

            ///TODO: Fix serializers because there are some mayor errors in working with them
            serializer.Update(o, ob.value());
            break;
        }
    } else {
        GameApi::log(ERR << "We have received message even when we are sending data not receiving");
    }
}


