//
// Created by Michal_Marszalek on 2021-02-06.
//

#include "Attacked.h"
#include <GameClient/Components/AttackShip.h>
#include <GameClient/Components/Enemy.h>

namespace AttackMachine {

    OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>> Attacked::handle(AttackedEvent &) {
        if (ship->parent->cell == mono_state::flee)
            return TransitionTo<FleeState>{};

        return TransitionTo<AttackState>{};
    }

    Maybe<TransitionTo<AttackState>> Attacked::handle(ChangedBaseModeEvent &) {
        switch (ship->parent->cell) {
            case mono_state::attack:
                return TransitionTo<AttackState>{};
            default:
                return Nothing{};
        }
    }

}
