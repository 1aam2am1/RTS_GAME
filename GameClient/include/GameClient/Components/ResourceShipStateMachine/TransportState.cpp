//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "TransportState.h"
#include "WaitState.h"
#include "AttackState.h"
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <Core/Time.h>
#include <GameClient/Components/ResourcePoint.h>

Maybe<TransitionTo<AttackState>> TransportState::handle(AttackedEvent &) {
    if (ship->parent->cell == mono_state::attack) {
        return TransitionTo<AttackState>{};
    }

    return Nothing{};
}

Maybe<TransitionTo<WaitState>> TransportState::handle(UpdateEvent &) {
    auto o2 = ship->transform()->localPosition() - ship->target->localPosition();
    auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y) - 1;

    if (l2 < ship->resource_area) {
        size_t empty = 0;
        for (auto &&res : ship->resources) {
            auto get = ship->speed_of_gathering * Time::deltaTime();
            if (get >= res.second) {
                get = res.second;
                ++empty;
            }

            ship->parent->resources[res.first] += get;

            res.second -= get;

        }
        if (empty == ship->resources.size()) {
            return TransitionTo<WaitState>{};
        }
    }

    return Nothing{};
}

void TransportState::onEnter() {
    auto &vec = ship->parent->buildings;

    ship->target = ship->parent->transform();
    double l;
    {
        auto o2 = ship->transform()->localPosition() - ship->target->localPosition();
        l = std::sqrt(o2.x * o2.x + o2.y * o2.y) - 1;
    }

    for (auto &v : vec) {
        auto point = v->GetComponent<ResourcePoint>();
        if (!point) { continue; }
        if (!point->isActiveAndEnabled()) { continue; }
        auto o2 = ship->transform()->localPosition() - v->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y) - 1;

        if (l > l2) {
            l = l2;
            ship->target = v->transform();
        }
    }
}
