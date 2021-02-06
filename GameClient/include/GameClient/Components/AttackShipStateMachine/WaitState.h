//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_WAITSTATE2_H
#define RTS_GAME_WAITSTATE2_H

#include <StateMachine.hpp>
#include "Event.h"
#include "Attacked.h"

namespace AttackMachine {

    struct AttackState;
    struct FleeState;
    struct Patrol;

    struct WaitState : Will<On<FixedUpdateEvent, Nothing>>, Attacked {
        using Will::handle;
        using Attacked::handle;

        template<class T>
        void onEnter(T &) {
            onEnter();
        }

        void onEnter();


        OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>, TransitionTo<Patrol>> handle(UpdateEvent &);
    };
}

#endif //RTS_GAME_WAITSTATE2_H
