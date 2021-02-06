//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "WaitState.h"
#include "FleeState.h"
#include "AttackState.h"
#include <GameClient/Components/AttackShip.h>
#include <GameClient/Components/Enemy.h>
#include <box2d/box2d.h>
#include <Physics2D/Physics2D.h>

namespace AttackMachine {

    OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>, TransitionTo<Patrol>>
    WaitState::handle(UpdateEvent &) {
        switch (ship->parent->cell) {
            case mono_state::attack:
                return TransitionTo<AttackState>{};
            case mono_state::flee:
                return TransitionTo<FleeState>{};
            case mono_state::wait:
            case mono_state::resource:
            default:
                return TransitionTo<Patrol>{};
        }
    }

    void WaitState::onEnter() {
        ship->target = nullptr;
    }

}
