//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "NetworkInterface.h"
#include "Synchronizer.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(NetworkInterface, server)

void NetworkInterface::Start() {
    auto vec = FindObjectsOfType<NetworkInterface>();

    if (vec.size() >= 2)
        GameApi::log(ERR << "NetworkInterface is singleton it should be only one.");
}

void NetworkInterface::Update() {
    if (socked.getLocalPort() == 0) { return; }

    for (auto it = ids2.begin(); it != ids2.end();) {
        if (it->second.expired()) {
            sf::Packet p;
            p << Synchronizer::Delete;
            p << it->first;

            it = ids2.erase(it);
        } else {
            ++it;
        }

    }

    //here send and receive messages
    for (auto &p : to_send) {
        auto status = socked.send(p);
        if (status == sf::Socket::Error || status == sf::Socket::Disconnected) {
            GameApi::log(ERR << "Network error send packed: " << status);
        }
    }
    to_send.clear();
    received.clear();

    //here read messages
    sf::Socket::Status status;
    do {
        sf::Packet p;
        status = socked.receive(p);
        if (status == sf::Socket::Done) {
            try {
                uint32_t typ;
                uint32_t id;
                size_t size;
                std::string syn_type;
                nlohmann::json json;
                p >> typ;
                p >> syn_type;
                p >> id;
                p >> size;

                auto data = reinterpret_cast<const uint8_t *>(static_cast<const char *>(p.getData()) +
                                                              p.getReadPosition());
                json = nlohmann::json::from_cbor(data, data + size);

                if (typ == Synchronizer::Update) {
                    auto o = dynamic_pointer_cast<Synchronizer>(GetObject(id));
                    if (!o) {
                        auto go = newGameObject();
                        o = dynamic_pointer_cast<Synchronizer>(
                                go->AddComponent(MetaData::getReflection(syn_type).type));
                        RegisterID(id, o);
                    }
                    if (o)
                        o->ReceiveMessage(json);

                } else if (typ == Synchronizer::Delete) {
                    auto o = GetObject(id);
                    Destroy(o);
                }

            } EXCEPTION_PRINT
        }
    } while (status == sf::Socket::Done);

}

void NetworkInterface::OnDestroy() {
    t.detach();
    Disconnect();
    ClosePort();
}

bool NetworkInterface::Connect(sf::IpAddress ip, int port) {
    listener.close();
    server = false;

    socked.setBlocking(true);
    bool result = socked.connect(ip, port, sf::seconds(1)) == sf::Socket::Done;
    socked.setBlocking(false);

    return result;
}

void NetworkInterface::Disconnect() {
    socked.disconnect();
}

bool NetworkInterface::OpenPort(int port) {
    socked.disconnect();
    listener.close();
    server = true;

    auto result = listener.listen(port) == sf::Socket::Done;
    t = std::thread([&]() {
        if (listener.accept(socked) != sf::Socket::Done) {
            GameApi::log(ERR << "Socked didn't connect");
        }

        socked.setBlocking(false);
    });

    return result;
}

void NetworkInterface::ClosePort() {
    listener.close();
}

bool NetworkInterface::isConnected() {
    return socked.getRemoteAddress() != sf::IpAddress::None;
}

bool NetworkInterface::IsOpenedPort() {
    return listener.getLocalPort() != 0;
}

uint32_t NetworkInterface::GetID(const TPtr<Object> &o) {
    auto it = ids.find(o.get());
    if (it != ids.end()) {
        return it->second;
    }
    if (server) {
        if (!(max_id & 1)) {
            max_id += 1; //server 1,3,5
        }
    } else {
        if (max_id & 1) {
            max_id += 1; //client 2,4,6
        }
    }

    max_id += 2;
    auto id = max_id;
    ids.emplace(o.get(), id);
    ids2.emplace(id, o);
    return id;
}

TPtr<Object> NetworkInterface::GetObject(uint32_t id) {
    auto it = ids2.find(id);

    if (it != ids2.end()) {
        return it->second;
    }

    return TPtr<Object>();
}

bool NetworkInterface::RegisterID(uint32_t id, const TPtr<Object> &o) {
    if (o.expired()) {
        return false;
    }

    auto it = ids.find(o.get());
    if (it != ids.end()) {
        return false;
    }

    auto it2 = ids2.find(id);
    if (it2 != ids2.end()) {
        return false;
    }

    ids.emplace(o.get(), id);
    ids2.emplace(id, o);

    return true;
}
