//
// Created by Michal_Marszalek on 2021-02-06.
//

#ifndef RTS_GAME_ATTACKED_H
#define RTS_GAME_ATTACKED_H

#include <StateMachine.hpp>
#include "Event.h"

namespace AttackMachine {
    struct AttackState;
    struct FleeState;

    struct Attacked {
        AttackShip *ship = nullptr;

        OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>> handle(AttackedEvent &e);

        Maybe<TransitionTo<AttackState>> handle(ChangedBaseModeEvent &e);
    };

}

#endif //RTS_GAME_ATTACKED_H
