//
// Created by Michal_Marszalek on 2021-02-06.
//

#include "Patrol.h"
#include <GameClient/Components/AttackShip.h>
#include <GameClient/Components/Enemy.h>

namespace AttackMachine {
    static std::random_device rd{};

    TransitionTo<GoToState> Patrol::handle(UpdateEvent &) {
        auto &b = ship->parent->buildings;

        if (!b.empty()) {
            std::uniform_int_distribution<> d(0, b.size() - 1);

            auto &where = b[d(rd)];

            ship->target_position_goto = where->transform()->localPosition();
        } else {
            ship->target_position_goto = ship->parent->transform()->localPosition();
        }

        std::uniform_int_distribution<> distribx(-5, 5);

        ship->target_position_goto.x += distribx(rd);
        ship->target_position_goto.y += distribx(rd);

        return TransitionTo<GoToState>();
    }

    void Patrol::onEnter() {
        ship->target = nullptr;
    }
}
