//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_NETWORKINTERFACE_H
#define RTS_GAME_NETWORKINTERFACE_H

#include <Core/MonoBehaviour.h>
#include <SFML/Network.hpp>

class NetworkInterface : public MonoBehaviour {
public:
    void Start() override;

    void OnDestroy() override;

    void Update() override;

    bool Connect(sf::IpAddress ip, int port = 22653);

    void Disconnect();

    bool OpenPort(int port = 22653);

    bool IsOpenedPort();

    void ClosePort();

    bool isConnected();

    bool isServer() { return server; }

private:
    friend class Synchronizer;

    bool server = false;
    std::thread t;

    sf::TcpListener listener;
    sf::TcpSocket socked;

    std::vector<sf::Packet> to_send;
    std::vector<sf::Packet> received;

    uint32_t GetID(const TPtr<Object> &);

    TPtr<Object> GetObject(uint32_t);

    bool RegisterID(uint32_t, const TPtr<Object> &);

    uint32_t max_id = 1;
    std::unordered_map<Object *, uint32_t> ids;
    std::unordered_map<uint32_t, TPtr<Object>> ids2;
};


#endif //RTS_GAME_NETWORKINTERFACE_H
