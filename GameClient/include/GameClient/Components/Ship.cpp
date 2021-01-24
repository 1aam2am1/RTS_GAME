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
#include <random>

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

void Ship::OnCollisionStay2D(const Collision2D &other) {
    if (other.transform != target && target && rigidbody && !other.rigidbody) {
        auto where = transform()->localPosition() - target->localPosition();

        auto length = (std::sqrt(where.x * where.x + where.y * where.y));
        where /= length;
        where *= force;

        rigidbody->AddForce({-where.y, where.x});
    }
}
