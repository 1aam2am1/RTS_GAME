//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "NetworkInterface.h"
#include "Synchronizer.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(NetworkInterface, server, print)

static TPtr<NetworkInterface> network_singleton;

const TPtr<NetworkInterface> &NetworkInterface::network() {
    return network_singleton;
}


void NetworkInterface::Start() {
    if (network_singleton) {
        GameApi::log(ERR << "NetworkInterface is singleton it should be only one.");
    } else {
        network_singleton = static_pointer_cast<NetworkInterface>(shared_from_this());
    }
}

void NetworkInterface::SendMessage(uint32_t id, std::string_view type, const nlohmann::json &message) {
    if (!isConnected()) { return; }

    sf::Socket::Status status;
    try {
        std::vector<std::uint8_t> v_msgpack = nlohmann::json::to_msgpack(message);

        sf::Packet p;
        p << id;
        p << type.data();
        p.append(v_msgpack.data(), v_msgpack.size());

        do {
            status = socked.send(p);
        } while (status == sf::Socket::Partial);

        if (status != sf::Socket::Done) {
            GameApi::log(ERR << "Network error send packed: " << status);
        }
    } EXCEPTION_PRINT

    if (print) {
        GameApi::log(INFO << message.dump(2, ' '));
    }
}

void NetworkInterface::Update() {
    if (!isConnected()) { return; }

    //here read messages
    sf::Socket::Status status;
    do {
        sf::Packet p;
        status = socked.receive(p);
        if (status == sf::Socket::Done) {
            try {
                uint32_t id;
                std::string type;
                nlohmann::json json;

                p >> id;
                p >> type;

                auto start = static_cast<const uint8_t *>(p.getData()) + p.getReadPosition();
                auto stop = static_cast<const uint8_t *>(p.getData()) + p.getDataSize();
                if (start != stop)
                    json = nlohmann::json::from_msgpack(start, stop);

                if (print) {
                    GameApi::log(INFO << json.dump(2, ' '));
                }

                auto it = receivers.find(id);
                if (it != receivers.end()) {

                    if (type == "__DELETE") {
                        Destroy(it->second);
                    } else if (it->second) {
                        it->second->ReceiveMessage(json);
                    } else {
                        GameApi::log(ERR << "Received message for deleted receiver.");
                    }
                } else {
                    auto go = newGameObject();
                    auto o = dynamic_pointer_cast<Synchronizer>(
                            go->AddComponent(MetaData::getReflection(type).type));
                    o->id = id;
                    o->ReceiveMessage(json);
                }

            } EXCEPTION_PRINT
        }
    } while (status == sf::Socket::Done);

}

void NetworkInterface::OnDestroy() {
    if (t.joinable()) { t.detach(); }
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
    if (t.joinable()) { t.detach(); }
    t = std::thread([&]() {
        if (listener.accept(socked) != sf::Socket::Done) {
            GameApi::log(ERR << "Socked didn't connect");
        }

        socked.setBlocking(false);
        listener.close();
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

uint32_t NetworkInterface::GetID() {
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
    return id;
}

void NetworkInterface::RegisterReceiver(uint32_t id, TPtr<Synchronizer> o) {
    if (!o) { return; }
    auto[it, b] = receivers.emplace(id, std::move(o));

    if (!b) {
        GameApi::log(ERR << "Receiver with " << id << " is registered");
    }
}

void NetworkInterface::RemoveReceiver(uint32_t id) {
    receivers.erase(id);
}
