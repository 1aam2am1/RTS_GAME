//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "ShipAi.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/GlobalStaticVariables.h>
#include "Enemy.h"
#include <box2d/box2d.h>

ADD_USER_COMPONENT(ShipAi, target, force, state, visibility, parent)

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

    switch (state) {
        case mono_state::attack: {
            for (auto &&s : r.shpis) {
                if (s && s->parent != parent) {
                    //Simple targeting
                    if (!target) {
                        target = s->transform();
                    } else {
                        auto o1 = transform()->localPosition() - target->localPosition();
                        auto o2 = transform()->localPosition() - s->transform()->localPosition();
                        auto l1 = std::sqrt(o1.x * o1.x + o1.y * o1.y);
                        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

                        if (l2 < l1) {
                            target = s->transform();
                        }
                    }
                }
            }
            if (!target) {
                break;
            }
        }
            [[fallthrough]];
        case mono_state::resource: {
            float value_points = 0;
            auto calculate_points = [&](const TPtr<Resource> &r) {
                if (!r) { return -1.0f; }
                auto o2 = transform()->localPosition() - r->transform()->localPosition();
                auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);


                if (parent->needed[r->type] >= 0.98f) {
                    return r->volume + 20 - l2 * 5.0f;
                }

                auto val = r->volume * parent->needed[r->type];

                return val - l2 * 10;
            };
            for (auto &&s : r.resources) {
                auto point = calculate_points(s);

                if (point > value_points || !target) {
                    value_points = point;
                    target = s->transform();
                }
            }
        }
            break;
        case mono_state::flee:
            target = parent->transform();
            break;
        case mono_state::wait:
            state = parent->cell;
            break;
    }
}

