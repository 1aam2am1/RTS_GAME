//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_GOTOSTATE_H
#define RTS_GAME_GOTOSTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct WaitState;
struct AttackState;
struct FleeState;

struct GoToState : ByDefault<Nothing> {
    ResourceShip *ship = nullptr;

    using ByDefault::handle;

    Maybe<TransitionTo<AttackState>, TransitionTo<FleeState>> handle(AttackedEvent &e);

    Maybe<TransitionTo<WaitState>> handle(FixedUpdateEvent &e);

};


#endif //RTS_GAME_GOTOSTATE_H
