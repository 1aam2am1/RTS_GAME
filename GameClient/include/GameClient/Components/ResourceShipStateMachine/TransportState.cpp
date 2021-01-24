//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "TransportState.h"
#include "WaitState.h"
#include "AttackState.h"
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <Core/Time.h>

Maybe<TransitionTo<AttackState>> TransportState::handle(AttackedEvent &) {
    if (ship->parent->cell == mono_state::attack) {
        return TransitionTo<AttackState>{};
    }

    return Nothing{};
}

Maybe<TransitionTo<WaitState>> TransportState::handle(UpdateEvent &) {
    ship->target = ship->parent->transform();

    auto o2 = ship->transform()->localPosition() - ship->target->localPosition();
    auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y) - 1;

    if (l2 < ship->resource_area) {
        size_t empty = 0;
        for (auto &&res : ship->resources) {
            auto get = ship->speed_of_gathering * Time::deltaTime();
            if (get > res.second) {
                get = res.second;
            }

            ship->parent->GetComponent<Base>()->resources[res.first] += get;

            res.second -= get;

            if (get < 0.1f) {
                ++empty;
            }
        }
        if (empty == ship->resources.size()) {
            return TransitionTo<WaitState>{};
        }
    }

    return Nothing{};
}

void TransportState::onEnter() {
    ship->target = ship->parent->transform();
}
