//
// Created by Michal_Marszalek on 2021-01-21.
//

#include "Ship.h"
#include <Macro.h>
#include <GameClient/GlobalStaticVariables.h>
#include "Enemy.h"
#include <box2d/box2d.h>
#include <Core/Time.h>
#include <numbers>

EXPORT_CLASS(Ship, parent, target, force, visibility, attack_force, life)

void Ship::Start() {
    rigidbody = GetComponent<Rigidbody2D>();
    rigidbody->drag = 1;

    if (!parent) {
        Destroy(transform());
    } else {
        parent->AddShip(shared_from_this());
    }
}

void Ship::Update() {
    OnUpdate();
    if (!parent) {
        Destroy(gameObject());
    }
};

void Ship::OnDestroy() {
    if (parent) {
        parent->RemoveShip(shared_from_this());
    }
}

void Ship::FixedUpdate() {
    OnFixedUpdate();
    if (target) {
        auto where = transform()->localPosition() - target->localPosition();

        transform()->localRotation = std::atan2(where.y, where.x) * 180.0f / std::numbers::pi + 90.0f;

        auto length = (std::sqrt(where.x * where.x + where.y * where.y));
        where /= length;
        if (length > 5.0f) {
            where *= force;
        } else {
            where *= force * length / 5.0f;
        }

        rigidbody->AddForce({-where.x, -where.y});
    }
}
/*
void Ship::GetTarget() {
    if (!parent) { return; }

    const auto calculate_points = [&](const TPtr<Resource> &r) {
        if (!r) { return -1.0f; }
        auto o2 = transform()->localPosition() - r->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);


        if (parent->needed[r->type] >= 0.98f) {
            return r->volume + 20 - l2 * 5.0f;
        }

        auto val = r->volume * parent->needed[r->type] * parent->needed[r->type];

        return val - l2 * l2 * 10;
    };

    struct R : b2QueryCallback {
        bool ReportFixture(b2Fixture *fixture) override {

            auto collider2d = dynamic_pointer_cast<Collider2D>(fixture->GetUserData().lock());
            if (!collider2d) { return true; }

            auto transform = collider2d->transform();

            if (transform) {
                auto ship = transform->GetComponent<Ship>();
                if (ship) {
                    shpis.emplace_back(ship);
                } else {
                    auto resource = transform->GetComponent<Resource>();

                    if (resource) {
                        resources.emplace_back(resource);
                    }
                }
            }

            return true;
        }

        std::list<TPtr<Ship>> shpis;
        std::list<TPtr<Resource>> resources;
    };

    R r;
    b2AABB v;

    v.lowerBound.x = transform()->localPosition().x - visibility;
    v.lowerBound.y = transform()->localPosition().y - visibility;

    v.upperBound.x = transform()->localPosition().x + visibility;
    v.upperBound.y = transform()->localPosition().y + visibility;

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
            for (auto &&s : r.resources) {
                auto point = calculate_points(s);

                if (point > value_points || !target) {
                    value_points = point;
                    target = s->transform();
                }
            }
            if (!target) {
                for (auto &&s : Resource::resources) {
                    auto point = calculate_points(s);

                    if (point > value_points || !target) {
                        value_points = point;
                        target = s->transform();
                    }
                }
                if (!target) {
                    if (state == parent->cell) {
                        target = parent->transform();
                    }
                    state = parent->cell;
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

void Ship::GetResourcesFromWorld() {
    if (target) {
        auto res = target->GetComponent<Resource>();
        if (res) {
            auto o2 = transform()->localPosition() - target->transform()->localPosition();
            auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

            if (l2 < resource_area) {
                auto get = speed_of_gathering * Time::fixedDeltaTime;
                if (get > res->volume) {
                    get = res->volume;
                }
                if (get > calculate_max()) {
                    get = calculate_max();
                }

                resources[res->type] += get;
                res->volume -= get;
                return;
            }
        }
    }


    struct R : b2QueryCallback {
        bool ReportFixture(b2Fixture *fixture) override {

            auto collider2d = dynamic_pointer_cast<Collider2D>(fixture->GetUserData().lock());
            if (!collider2d) { return true; }

            auto transform = collider2d->transform();

            if (transform) {
                auto resource = transform->GetComponent<Resource>();

                if (resource) {
                    resources.emplace_back(resource);
                }
            }

            return true;
        }

        std::list<TPtr<Resource>> resources;
    };

    R r;
    b2AABB v;

    v.lowerBound.x = transform()->localPosition().x - resource_area;
    v.lowerBound.y = transform()->localPosition().y - resource_area;

    v.upperBound.x = transform()->localPosition().x + resource_area;
    v.upperBound.y = transform()->localPosition().y + resource_area;

    global.physics.world.QueryAABB(&r, v);

    std::erase_if(r.resources, [&](auto &&s) {
        if (!s || !s->transform()) { return true; }

        auto o2 = transform()->localPosition() - s->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

        if (l2 > resource_area) {
            return true;
        }
        return false;
    });

    const auto calculate_points = [&](const TPtr<Resource> &r) {
        if (!r) { return -1.0f; }
        auto o2 = transform()->localPosition() - r->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);


        if (parent->needed[r->type] >= 0.98f) {
            return r->volume + 20 - l2 * 5.0f;
        }

        auto val = r->volume * parent->needed[r->type] * parent->needed[r->type];

        return val - l2 * l2 * 10;
    };

    TPtr<Resource> best;
    float value_points = 0;

    for (auto &&res : r.resources) {
        auto point = calculate_points(res);

        if (point > value_points || !best) {
            value_points = point;
            best = res;
        }
    }

    if (best) {
        auto get = speed_of_gathering * Time::fixedDeltaTime;
        if (get > best->volume) {
            get = best->volume;
        }
        if (get > calculate_max()) {
            get = calculate_max();
        }

        resources[best->type] += get;
        best->volume -= get;
    }
}

float Ship::calculate_max() {
    auto result = resources[ResourceType::water] + resources[ResourceType::metal] + resources[ResourceType::food];
    result = max_resources - result;

    if (result > 0) {
        return result;
    }

    return 0.f;
}

//TODO: !!! Better state transition
void Ship::PlaceResourceInBase() {
    if (target && calculate_max() != max_resources) {
        auto o2 = transform()->localPosition() - parent->transform()->localPosition();
        auto l2 = std::sqrt(o2.x * o2.x + o2.y * o2.y);

        if ((l2 - 1) < resource_area) {
            for (auto &&res : resources) {
                auto get = speed_of_gathering * Time::fixedDeltaTime;
                if (get > res.second) {
                    get = res.second;
                }

                parent->GetComponent<Base>()->resources[res.first] += get;

                res.second -= get;
            }
        }
    } else if (calculate_max() == max_resources) {
        state = mono_state::wait;
    }
}
*/
