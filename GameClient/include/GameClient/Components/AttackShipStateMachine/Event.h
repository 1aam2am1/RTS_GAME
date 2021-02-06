//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_EVENT2_H
#define RTS_GAME_EVENT2_H


#include <utility>
#include <GameClient/TPtr.h>

class AttackShip;
namespace AttackMachine {

    struct Event {

    };

    struct AttackedEvent : Event {
    };

    struct ChangedBaseModeEvent : Event {
    };

    struct UpdateEvent : Event {
    };

    struct FixedUpdateEvent : Event {
    };
}

#endif //RTS_GAME_EVENT2_H
