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

void Synchronizer::SendMessage(const nlohmann::json &j) {
    if (!NetworkInterface::network()) { return; }

    NetworkInterface::network()->SendMessage(id, GameApi::demangle(typeid(*this).name()), j);
}

bool Synchronizer::RegisterID(uint32_t s_id, const TPtr<Object> &o) {
    return GameObjectDatabase::Register(o, s_id);
}
