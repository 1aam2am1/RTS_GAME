//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "Synchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>

EXPORT_CLASS(Synchronizer, network)

Synchronizer::Synchronizer() = default;


void Synchronizer::Start() {
    auto vec = FindObjectsOfType<NetworkInterface>();
    if (!vec.empty())
        network = vec[0];

    if (!network)
        Destroy(shared_from_this());
}

bool Synchronizer::isServer() {
    return network ? network->server : false;
}

uint32_t Synchronizer::GetID(const TPtr<Object> &o) {
    return network->GetID(o);
}

TPtr<Object> Synchronizer::GetObject(uint32_t id) {
    return network->GetObject(id);
}

void Synchronizer::SendMessage(const nlohmann::json &j) {
    sf::Packet p;
    p << Update;
    p << GameApi::demangle(typeid(*this).name());
    p << GetID(shared_from_this());

    std::vector<uint8_t> v = nlohmann::json::to_cbor(j);
    p << v.size();
    p.append(v.data(), v.size());

    network->to_send.emplace_back(std::move(p));
}

bool Synchronizer::RegisterID(uint32_t id, const TPtr<Object> &o) {
    return network->RegisterID(id, o);
}

