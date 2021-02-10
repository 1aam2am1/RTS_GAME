//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_NETWORKINTERFACE_H
#define RTS_GAME_NETWORKINTERFACE_H

#include <Core/MonoBehaviour.h>
#include <SFML/Network.hpp>

class Synchronizer;

class NetworkInterface : public MonoBehaviour {
    void Start() override;

    void OnDestroy() override;

    void Update() override;

public:
    /// Connect with user
    bool Connect(sf::IpAddress ip, int port = 22653);

    /// Disconnect from user
    void Disconnect();

    /// Open server port
    bool OpenPort(int port = 22653);

    /// Is server opened
    bool IsOpenedPort();

    /// Close Server
    void ClosePort();

    /// Is Client Connected or are we connected to server
    bool isConnected();

    /// Are we server
    bool isServer() const { return server; }

    static const TPtr<NetworkInterface> &network();

private:
    friend class Synchronizer;

    uint32_t GetID();

    void SendMessage(uint32_t id, std::string_view type, const nlohmann::json &message);

    void RegisterReceiver(uint32_t id, TPtr<Synchronizer>);

    void RemoveReceiver(uint32_t id);

private:
    bool server = false;
    std::thread t;

    sf::TcpListener listener;
    sf::TcpSocket socked;

    uint32_t max_id = 100;

    std::unordered_map<uint32_t, TPtr<Synchronizer>> receivers;

    bool print = false;
};


#endif //RTS_GAME_NETWORKINTERFACE_H
