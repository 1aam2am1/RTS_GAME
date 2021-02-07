//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_SIGNALSYNCHRONIZER_H
#define RTS_GAME_SIGNALSYNCHRONIZER_H


#include "Synchronizer.h"
#include <GameApi/Signal.h>
#include <nlohmann/json.hpp>

class SignalSynchronizer : public Synchronizer {
public:
    sigslot::signal<int, nlohmann::json> OnMessage;

    void SendMessage(int, nlohmann::json);

    void Update() override;

protected:
    void ReceiveMessage(const nlohmann::json &json) override;
};


#endif //RTS_GAME_SIGNALSYNCHRONIZER_H
