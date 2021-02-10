//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "SignalSynchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(SignalSynchronizer)

void SignalSynchronizer::Update() {

}

void SignalSynchronizer::ReceiveMessage(const nlohmann::json &json) {
    auto id = json["__ID"].get<int>();
    auto copy = json;
    copy.erase("__ID");

    OnMessage(id, copy);
}

void SignalSynchronizer::SendMessage(int id, nlohmann::json json) {
    json["__ID"] = id;
    Synchronizer::SendMessage(json);
}

void SignalSynchronizer::Start() {
    Synchronizer::Start();

    if (send) {
        //Crete reflection
        FullSynchronizer::Update();
    }
    send = false;
}
