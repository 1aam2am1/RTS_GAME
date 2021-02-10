//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "Synchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>

EXPORT_CLASS(Synchronizer, id)

Synchronizer::Synchronizer() = default;


void Synchronizer::Awake() {
    if (!NetworkInterface::network())
        Destroy(shared_from_this());
}

void Synchronizer::Start() {
    if (!NetworkInterface::network()) { return; }

    if (id == 0) {
        id = NetworkInterface::network()->GetID();
    }

    NetworkInterface::network()->RegisterReceiver(id, shared_from_this());
}

bool Synchronizer::isServer() {
    return NetworkInterface::network() ? NetworkInterface::network()->isServer() : false;
}

uint32_t Synchronizer::GetID(const TPtr<Object> &o) {
    if (o.expired()) {
        return 0;
    }

    auto it = ids.find(o.get());
    if (it != ids.end()) {
        return it->second;
    }

    max_id += 1;
    auto new_id = max_id;
    ids.emplace(o.get(), new_id);
    ids2.emplace(new_id, o);
    return new_id;
}

TPtr<Object> Synchronizer::GetObject(uint32_t s_id) {
    auto it = ids2.find(s_id);
    if (it != ids2.end()) {
        return it->second;
    }

    return {};
}

void Synchronizer::SendMessage(const nlohmann::json &j) {
    if (!NetworkInterface::network()) { return; }

    NetworkInterface::network()->SendMessage(id, GameApi::demangle(typeid(*this).name()), j);
}

bool Synchronizer::RegisterID(uint32_t s_id, const TPtr<Object> &o) {
    if (o.expired()) {
        return false;
    }

    auto it = ids.find(o.get());
    if (it != ids.end()) {
        return false;
    }

    auto it2 = ids2.find(s_id);
    if (it2 != ids2.end()) {
        return false;
    }

    ids.emplace(o.get(), s_id);
    ids2.emplace(s_id, o);

    return true;
}
