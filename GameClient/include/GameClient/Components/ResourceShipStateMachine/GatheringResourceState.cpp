//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "GatheringResourceState.h"
#include <box2d/box2d.h>
#include <GameClient/Components/Resource.h>
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <Core/Time.h>
#include <Physics2D/Physics2D.h>

void GatheringResourceState::onEnter() {
    std::vector<TPtr<Resource>> resources;
    auto coliders = Physics2D::OverlapCircleAll(
            {ship->transform()->localPosition().x, ship->transform()->localPosition().y}, ship->visibility);
    for (auto &&c : coliders) {
        auto s = c->GetComponent<Resource>();
        if (s) {
            resources.emplace_back(s);
        }
    }

    const auto calculate_points = [&](const TPtr<Resource> &r) {
        if (!r) { return -1.0f; }
        auto o2 = ship->transform()->localPosition() - r->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);


        if (ship->parent->needed[r->type] >= 0.98f) {
            return r->GetVolume() + 20 - l2 * 5.0f;
        }

        auto val = r->GetVolume() * ship->parent->needed[r->type] * ship->parent->needed[r->type];

        return val - l2 * l2 * 10;
    };


    float value_points = 0;

    for (auto &&res : resources) {
        auto point = calculate_points(res);

        if (point > value_points || !ship->target) {
            value_points = point;
            ship->target = res->transform();
        }
    }
}

Maybe<TransitionTo<TransportState>, TransitionTo<GoToState>> GatheringResourceState::handle(UpdateEvent &) {
    if (!ship->target) {
        const auto calculate_points = [&](const TPtr<Resource> &r) {
            if (!r) { return -1.0f; }
            auto o2 = ship->transform()->localPosition() - r->transform()->localPosition();
            auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);


            if (ship->parent->needed[r->type] >= 0.98f) {
                return r->GetVolume() + 20 - l2 * 5.0f;
            }

            auto val = r->GetVolume() * ship->parent->needed[r->type] * ship->parent->needed[r->type];

            return val - l2 * l2 * 10;
        };

        float value_points = -FLT_MAX;
        for (auto &&res : Resource::resources) {
            auto point = calculate_points(res);

            if (point > value_points) {
                value_points = point;
                ship->target_position_goto = res->transform()->localPosition();
            }
        }
        return TransitionTo<GoToState>{};
    }

    auto res = ship->target->GetComponent<Resource>();
    if (res) {
        auto o2 = ship->transform()->localPosition() - ship->target->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

        if (l2 < ship->resource_area) {
            auto get = ship->speed_of_gathering * Time::deltaTime();
            if (get > res->GetVolume()) {
                get = res->GetVolume();
            }

            ship->resources[res->type] += get;
            res->RemoveFromVolume(get);

        }

        float cap = 0;
        for (auto &r : ship->resources) {
            cap += r.second;
        }
        if (cap >= ship->capacity) {
            return TransitionTo<TransportState>{};
        }
    }

    return Nothing{};
}

Maybe<TransitionTo<AttackState>, TransitionTo<FleeState>> GatheringResourceState::handle(AttackedEvent &) {
    if (ship->parent->cell == mono_state::attack) {
        return TransitionTo<AttackState>{};
    }

    return TransitionTo<FleeState>{};
}
