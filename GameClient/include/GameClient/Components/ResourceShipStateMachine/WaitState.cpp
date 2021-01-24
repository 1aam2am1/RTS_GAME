//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "WaitState.h"
#include "FleeState.h"
#include "AttackState.h"
#include "GatheringResourceState.h"
#include <GameClient/Components/ResourceShip.h>
#include <GameClient/Components/Enemy.h>
#include <box2d/box2d.h>
#include <Physics2D/Physics2D.h>

OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>> WaitState::handle(AttackedEvent &) {
    if (ship->parent->cell == mono_state::attack)
        return TransitionTo<AttackState>{};

    return TransitionTo<FleeState>{};
}

OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>, TransitionTo<GatheringResourceState>>
WaitState::handle(UpdateEvent &) {
    switch (ship->parent->cell) {
        case mono_state::attack: {
            std::vector<TPtr<Ship>> allships;
            auto coliders = Physics2D::OverlapCircleAll(
                    {ship->transform()->localPosition().x, ship->transform()->localPosition().y}, ship->visibility);
            for (auto &&c : coliders) {
                auto s = c->GetComponent<Ship>();
                if (s) {
                    allships.emplace_back(s);
                }
            }

            std::erase_if(allships, [this](auto &&s) { return s->parent == ship->parent; });

            if (!allships.empty()) {
                return TransitionTo<AttackState>{};
            } else {
                return TransitionTo<GatheringResourceState>{};
            }
        }
        case mono_state::flee:
            return TransitionTo<FleeState>{};
        case mono_state::wait:
        case mono_state::resource:
        default:
            return TransitionTo<GatheringResourceState>{};
    }
}

void WaitState::onEnter() {
    ship->target = nullptr;
}
