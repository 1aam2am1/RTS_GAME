//
// Created by Michal_Marszalek on 2021-02-07.
//

#include "SignalSynchronizer.h"
#include "NetworkInterface.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(SignalSynchronizer)

void SignalSynchronizer::IntUpdate() {
    if (send) {
        //Crete reflection
        if (FullSynchronizer::SendStatus()) {
            send = false;
        }
    }
}

void SignalSynchronizer::Start() {
    Synchronizer::Start();

    SignalSynchronizer::Update();
}
