//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "AttackState.h"
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <Physics2D/Physics2D.h>

Maybe<TransitionTo<WaitState>> AttackState::handle(UpdateEvent &) {
    if (!ship->target) {
        std::vector<TPtr<Ship>> shpis;
        auto coliders = Physics2D::OverlapCircleAll(
                {ship->transform()->localPosition().x, ship->transform()->localPosition().y}, ship->visibility);
        for (auto &&c : coliders) {
            auto s = c->GetComponent<Ship>();
            if (s) {
                shpis.emplace_back(s);
            }
        }

        float how_far = 0;
        for (auto &&s : shpis) {
            if (s && s->parent != ship->parent) {
                //Simple targeting
                if (!ship->target) {
                    ship->target = s->transform();
                    auto o1 = ship->transform()->localPosition() - ship->target->localPosition();
                    how_far = std::sqrt(o1.x * o1.x + o1.y * o1.y);
                } else {
                    auto o2 = ship->transform()->localPosition() - s->transform()->localPosition();
                    auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

                    if (l2 < how_far) {
                        ship->target = s->transform();
                    }
                }
            }
        }

        if (!ship->target) { return TransitionTo<WaitState>{}; }
    }
    return Nothing{};
}

Maybe<TransitionTo<WaitState>> AttackState::handle(FixedUpdateEvent &) {
    ///TODO: Create bullets
    return Nothing{};
}

Maybe<TransitionTo<WaitState>> AttackState::handle(ChangedBaseModeEvent &) {
    if (ship->parent->cell == mono_state::attack)
        return Nothing{};

    return TransitionTo<WaitState>{};
}

void AttackState::onEnter() {
    ship->target = nullptr;
    //TODO: Mark target
}
