//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_ATTACKSTATE_H
#define RTS_GAME_ATTACKSTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct WaitState;

struct AttackState : Will<On<AttackedEvent, Nothing>> {
    ResourceShip *ship = nullptr;
    float time_to_attack = 0;

    using Will::handle;

    template<class T>
    void onEnter(T &) {
        onEnter();
    }

    void onEnter();

    Maybe<TransitionTo<WaitState>> handle(UpdateEvent &e);

    Maybe<TransitionTo<WaitState>> handle(FixedUpdateEvent &e);

    Maybe<TransitionTo<WaitState>> handle(ChangedBaseModeEvent &e);

private:

};


#endif //RTS_GAME_ATTACKSTATE_H
