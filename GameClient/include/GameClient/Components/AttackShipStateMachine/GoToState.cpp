//
// Created by Michal_Marszalek on 2021-01-24.
//

#include "GoToState.h"
#include <GameClient/Components/AttackShip.h>
#include <GameClient/Components/Enemy.h>
#include <numbers>
#include <Core/Time.h>

namespace AttackMachine {

    Maybe<TransitionTo<WaitState>> GoToState::handle(FixedUpdateEvent &) {
        t += Time::fixedDeltaTime;

        auto where = ship->target_position_goto;
        where.x -= ship->transform()->localPosition().x;
        where.y -= ship->transform()->localPosition().y;

        auto length = (std::sqrt(where.x * where.x + where.y * where.y));
        where /= length;

        if (length > 0.15f) {
            ship->transform()->localRotation = std::atan2(where.y, where.x) * 180.0f / std::numbers::pi - 90.0f;
        }

        if (length < 10.0f) {
            where *= ship->force / 3.0f;
        }

        if (length < 5.f) {
            where *= length / 5.0f;
        }
        if (length < 0.15f) {
            where = {0.f, 0.f, 0.f};
        }

        if (length < 0.5f && t >= 2) {
            return TransitionTo<WaitState>{};
        } else if (t >= 10) {
            return TransitionTo<WaitState>{};
        }

        ship->rigidbody->AddForce({where.x, where.y});

        return Nothing{};
    }
}
