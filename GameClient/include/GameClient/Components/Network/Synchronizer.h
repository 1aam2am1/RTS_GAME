//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_SYNCHRONIZER_H
#define RTS_GAME_SYNCHRONIZER_H

#include <Core/MonoBehaviour.h>
#include <GameClient/GuidFileIdPack.h>

class Synchronizer : public MonoBehaviour {
public:
    Synchronizer();

    void Awake() final;

    void Start() override;

protected:

    bool isServer();

    GUIDFileIDPack GetID(const TPtr<Object> &);

    TPtr<Object> GetObject(GUIDFileIDPack);

    bool RegisterID(uint32_t, const TPtr<Object> &);

    void SendMessage(const nlohmann::json &);

    virtual void ReceiveMessage(const nlohmann::json &) = 0;

private:
    friend class NetworkInterface;

    uint32_t id = 0;
};


#endif //RTS_GAME_SYNCHRONIZER_H
