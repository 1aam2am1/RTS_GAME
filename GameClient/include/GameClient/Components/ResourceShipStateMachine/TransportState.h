//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_TRANSPORTSTATE_H
#define RTS_GAME_TRANSPORTSTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct AttackState;
struct WaitState;

struct TransportState : Will<On<FixedUpdateEvent, Nothing>, On<ChangedBaseModeEvent, Nothing>> {
    ResourceShip *ship = nullptr;

    using Will::handle;

    template<class T>
    void onEnter(T &) {
        onEnter();
    }

    void onEnter();

    Maybe<TransitionTo<AttackState>> handle(AttackedEvent &e);

    Maybe<TransitionTo<WaitState>> handle(UpdateEvent &e);
};


#endif //RTS_GAME_TRANSPORTSTATE_H
