//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_SYNCHRONIZER_H
#define RTS_GAME_SYNCHRONIZER_H

#include <Core/MonoBehaviour.h>
#include "NetworkInterface.h"


class Synchronizer : public MonoBehaviour {
public:
    Synchronizer();

    void Start() final;

    bool isServer();

    uint32_t GetID(const TPtr<Object> &);

    TPtr<Object> GetObject(uint32_t);

    bool RegisterID(uint32_t, const TPtr<Object> &);

protected:
    friend class NetworkInterface;

    TPtr<NetworkInterface> network;

    void SendMessage(const nlohmann::json &);

    virtual void ReceiveMessage(const nlohmann::json &) = 0;

    enum Type : uint32_t {
        Update = 0,
        Delete = 1,
    };
};


#endif //RTS_GAME_SYNCHRONIZER_H
