//
// Created by Michal_Marszalek on 2021-02-06.
//

#include "AttackState.h"
#include <GameClient/Components/AttackShip.h>
#include <GameClient/Components/Enemy.h>
#include <Physics2D/Physics2D.h>
#include <Core/Time.h>
#include <GameClient/Components/PrefabFunc.h>
#include <numbers>
#include <GameClient/Components/Life/Attack.h>

namespace AttackMachine {

    Maybe<TransitionTo<WaitState>> AttackState::handle(UpdateEvent &) {
        if (!ship->target && ship->parent->other_enemy) {
            const std::vector<TPtr<Ship>> &s = ship->parent->other_enemy->ships;

            auto search = [&](auto &ships, auto f) {
                float how_far = 0;
                for (auto &&s : ships) {
                    //Simple targeting
                    if (!ship->target) {
                        ship->target = s->transform();
                        auto o1 = ship->transform()->localPosition() - ship->target->localPosition();
                        how_far = std::sqrt(o1.x * o1.x + o1.y * o1.y);
                        how_far -= f(ship->target);
                    } else {
                        auto o2 = ship->transform()->localPosition() - s->transform()->localPosition();
                        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

                        if (l2 < how_far) {
                            ship->target = s->transform();
                            how_far = l2;
                            how_far -= f(ship->target);
                        }
                    }
                }
            };

            search(s, [](auto &target) { return 10 * (target->template GetComponent<AttackShip>() ? 1 : 0); });

            if (!ship->target) {
                const auto &buildings = ship->parent->other_enemy->buildings;

                search(buildings, [](auto &target) {
                    auto b = target->template GetComponent<Building>();
                    if (!b) { return 0; }
                    return 10 * (b->type == ShipType::Attack ? 1 : 0);
                });
            }
        }
        return Nothing{};
    }

    Maybe<TransitionTo<WaitState>> AttackState::handle(FixedUpdateEvent &) {
        time_to_attack -= Time::fixedDeltaTime;
        if (ship->target && time_to_attack <= 0) {
            auto position = ship->transform()->localPosition();
            auto rot = ship->transform()->localRotation();
            auto radians = rot * std::numbers::pi / 180.0f;

            constexpr float bullet_size = 1;

            position.x -= bullet_size * sin(radians);
            position.y += bullet_size * cos(radians);

            auto bullet = Prefab_func::create_bullet(ship->gameObject(), position, rot);
            bullet->GetComponent<Attack>()->attack = ship->attack_force;

            time_to_attack = 2;
        }

        return Nothing{};
    }

    Maybe<TransitionTo<WaitState>> AttackState::handle(ChangedBaseModeEvent &) {
        if (ship->parent->cell == mono_state::flee)
            return TransitionTo<WaitState>{};

        return Nothing{};
    }

    void AttackState::onEnter() {
        ship->target = nullptr;
        //TODO: Mark target
    }
}

