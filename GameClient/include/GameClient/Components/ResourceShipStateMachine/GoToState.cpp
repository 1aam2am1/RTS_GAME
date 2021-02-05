//
// Created by Michal_Marszalek on 2021-01-24.
//

#include "GoToState.h"
#include <GameClient/Components/Resource.h>
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <numbers>

Maybe<TransitionTo<WaitState>> GoToState::handle(FixedUpdateEvent &) {
    auto where = ship->target_position_goto;
    where.x -= ship->transform()->localPosition().x;
    where.y -= ship->transform()->localPosition().y;

    ship->transform()->localRotation = std::atan2(where.y, where.x) * 180.0f / std::numbers::pi - 90.0f;

    auto length = (std::sqrt(where.x * where.x + where.y * where.y));
    where /= length;
    if (length > 5.0f) {
        where *= ship->force;
    } else {
        where *= ship->force * length / 5.0f;
        return TransitionTo<WaitState>{};
    }

    ship->rigidbody->AddForce({where.x, where.y});

    return Nothing{};
}

Maybe<TransitionTo<AttackState>, TransitionTo<FleeState>> GoToState::handle(AttackedEvent &) {
    if (ship->parent->cell == mono_state::attack) {
        return TransitionTo<AttackState>{};
    }

    return TransitionTo<FleeState>{};
}
