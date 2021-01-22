//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "ShipAi.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/GlobalStaticVariables.h>
#include "Enemy.h"
#include <box2d/box2d.h>

ADD_USER_COMPONENT(ShipAi, target, force, state, visibility)

void ShipAi::FindNext() {

}

void ShipAi::GetTarget() {
    if (!parent) { return; }

    parent->cell;


    struct R : b2QueryCallback {
        bool ReportFixture(b2Fixture *fixture) override {

            auto transform = dynamic_pointer_cast<Transform>(fixture->GetBody()->GetUserData().lock());
            if (transform) {
                auto ship = transform->GetComponent<ShipAi>();
                if (ship) {
                    shpis.emplace_back(ship);
                } else {
                    auto resource = transform->GetComponent<Resource>();

                    if (resource) {
                        resources.emplace_back(resource);
                    }
                }
            }

            return false;
        }

        std::list<TPtr<ShipAi>> shpis;
        std::list<TPtr<Resource>> resources;
    };

    R r;
    b2AABB v;
    global.physics.world.QueryAABB(&r, v);

    //TODO: Ai logika rozmyta

}

