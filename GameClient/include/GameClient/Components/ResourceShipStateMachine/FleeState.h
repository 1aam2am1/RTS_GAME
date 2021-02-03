//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_FLEESTATE_H
#define RTS_GAME_FLEESTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct WaitState;
struct AttackState;

struct FleeState : Will<On<UpdateEvent, Nothing>> {
    ResourceShip *ship = nullptr;
    float not_attacked = 0;

    using Will::handle;

    template<class T>
    void onEnter(T &) {
        onEnter();
    }

    void onEnter();

    Maybe<TransitionTo<WaitState>> handle(FixedUpdateEvent &e);

    TransitionTo<FleeState> handle(AttackedEvent &) {
        not_attacked = 0;
        return {};
    }

    Maybe<TransitionTo<AttackState>, TransitionTo<WaitState>> handle(ChangedBaseModeEvent &e);
};


#endif //RTS_GAME_FLEESTATE_H
