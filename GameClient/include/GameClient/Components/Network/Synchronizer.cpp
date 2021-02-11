//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "Synchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>
#include <GameClient/GameObjectDatabase.h>

EXPORT_CLASS(Synchronizer)

Synchronizer::Synchronizer() = default;


void Synchronizer::Awake() {
    if (!NetworkInterface::network())
        Destroy(shared_from_this());
}

void Synchronizer::Start() {
    if (!NetworkInterface::network()) { return; }

    if (id == 0) {
        id = GetID(gameObject()).id;
    }

    NetworkInterface::network()->RegisterReceiver(id, shared_from_this());
}

bool Synchronizer::isServer() {
    return NetworkInterface::network() ? NetworkInterface::network()->isServer() : false;
}

GUIDFileIDPack Synchronizer::GetID(const TPtr<Object> &obj) {
    GUIDFileIDPack pack;
    GameObjectDatabase::TryGetGUIDAndLocalFileIdentifier(obj, pack.guid, pack.id);
    return pack;
}

TPtr<Object> Synchronizer::GetObject(GUIDFileIDPack s_id) {
    return GameObjectDatabase::TRYGetObjectFROMGUIDAndLocalFileIdentifier(s_id.guid, s_id.id);
}

bool Synchronizer::SendMessage(const nlohmann::json &j) {
    if (!NetworkInterface::network()) { return false; }

    return NetworkInterface::network()->SendMessage(id, GameApi::demangle(typeid(*this).name()), j);
}

bool Synchronizer::RegisterID(uint32_t s_id, const TPtr<Object> &o) {
    return GameObjectDatabase::Register(o, s_id);
}

void Synchronizer::OnDestroy() {
    if (!NetworkInterface::network()) { return; }

    NetworkInterface::network()->SendMessage(id, "__DELETE", {});
}

void Synchronizer::SendMessage(int sid, nlohmann::json json) {
    json["__ID"] = sid;
    auto result = Synchronizer::SendMessage(json);

    if (!result) {
        to_send.emplace_back(std::move(json));
    }
}

void Synchronizer::Update() {
    std::erase_if(to_send, [&](auto &j) { return SendMessage(j); });

    IntUpdate();
}

void Synchronizer::ReceiveMessageInterface(const nlohmann::json &json) {
    if (json.contains("__ID")) {
        auto sid = json.at("__ID").get<int>();
        auto copy = json;
        copy.erase("__ID");

        OnMessage(sid, copy);
    } else {
        ReceiveMessage(json);
    }
}
