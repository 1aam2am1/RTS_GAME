//
// Created by Michal_Marszalek on 2021-02-06.
//

#ifndef RTS_GAME_PATROL_H
#define RTS_GAME_PATROL_H


#include <random>
#include "Attacked.h"

namespace AttackMachine {

    struct GoToState;

    struct Patrol : Will<On<FixedUpdateEvent, Nothing>>, Attacked {
        using Will::handle;
        using Attacked::handle;

        template<class T>
        void onEnter(T &) {
            onEnter();
        }

        void onEnter();

        TransitionTo<GoToState> handle(UpdateEvent &);
    };

}

#endif //RTS_GAME_PATROL_H
