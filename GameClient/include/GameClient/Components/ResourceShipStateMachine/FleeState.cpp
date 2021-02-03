//
// Created by Michal_Marszalek on 2021-01-23.
//

#include <Core/Time.h>
#include "FleeState.h"
#include "WaitState.h"
#include "AttackState.h"
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>

Maybe<TransitionTo<WaitState>> FleeState::handle(FixedUpdateEvent &) {
    not_attacked += Time::fixedDeltaTime;
    ship->target = ship->parent->transform();

    if (not_attacked > ship->time_of_flee) {
        return TransitionTo<WaitState>{};
    }

    return Nothing{};
}

Maybe<TransitionTo<AttackState>, TransitionTo<WaitState>> FleeState::handle(ChangedBaseModeEvent &) {
    switch (ship->parent->cell) {
        case mono_state::attack:
            return TransitionTo<AttackState>{};
        default:
            return Nothing{};
    }
}

void FleeState::onEnter() {
    not_attacked = 0;
    ship->target = ship->parent->transform();
}
