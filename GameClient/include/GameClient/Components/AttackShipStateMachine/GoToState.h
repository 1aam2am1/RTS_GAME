//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_GOTOSTATE2_H
#define RTS_GAME_GOTOSTATE2_H

#include <StateMachine.hpp>
#include "Event.h"
#include "Attacked.h"

namespace AttackMachine {

    struct WaitState;
    struct AttackState;
    struct FleeState;

    struct GoToState : ByDefault<Nothing>, Attacked {
        using ByDefault::handle;
        using Attacked::handle;

        float t = 0;

        template<typename T>
        void onEnter(T &) {
            t = 0;
        }

        Maybe<TransitionTo<WaitState>> handle(FixedUpdateEvent &e);
    };
}

#endif //RTS_GAME_GOTOSTATE2_H
