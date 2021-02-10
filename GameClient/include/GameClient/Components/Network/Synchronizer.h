//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_SYNCHRONIZER_H
#define RTS_GAME_SYNCHRONIZER_H

#include <Core/MonoBehaviour.h>

class Synchronizer : public MonoBehaviour {
public:
    Synchronizer();

    void Awake() final;

    void Start() override;

protected:

    bool isServer();

    uint32_t GetID(const TPtr<Object> &);

    TPtr<Object> GetObject(uint32_t);

    bool RegisterID(uint32_t, const TPtr<Object> &);

    void SendMessage(const nlohmann::json &);

    virtual void ReceiveMessage(const nlohmann::json &) = 0;

private:
    friend class NetworkInterface;

    uint32_t id = 0;

    uint32_t max_id = 1;
    std::unordered_map<Object *, uint32_t> ids;
    std::unordered_map<uint32_t, TPtr<Object>> ids2;
};


#endif //RTS_GAME_SYNCHRONIZER_H
