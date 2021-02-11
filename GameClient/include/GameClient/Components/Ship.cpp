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

EXPORT_CLASS(Ship, parent, target, force, visibility, attack_force)

void Ship::Start() {
    rigidbody = GetComponent<Rigidbody2D>();
    rigidbody->drag = 1;

    if (!parent) {
        Destroy(transform());
        GameApi::log(ERR << "Ship needs parent");
    } else {
        parent->AddShip(shared_from_this());
        gameObject()->layer = parent->gameObject()->layer;
    }

    gameObject()->AddComponent<ShipDummy>()->parent = parent;
}

void Ship::Update() {
    if (!parent) {
        Destroy(gameObject());
        GameApi::log(ERR << "Ship needs parent");
    } else {
        OnUpdate();
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
        auto where = target->localPosition() - transform()->localPosition();

        transform()->localRotation = std::atan2(where.y, where.x) * 180.0f / std::numbers::pi - 90.0f;

        auto length = (std::sqrt(where.x * where.x + where.y * where.y));
        where /= length;
        if (length > 5.0f) {
            where *= force;
        } else {
            where *= force * length / 5.0f;
        }

        rigidbody->AddForce({where.x, where.y});
    }
}

void Ship::OnCollisionStay2D(const Collision2D &other) {
    if (other.transform != target && target && rigidbody) {

        auto where = target->localPosition() - transform()->localPosition();
        const auto r1 = std::atan2(where.y, where.x);
        const auto r2 = std::atan2(other.normal.y, other.normal.x);
        auto r3 = (r2 - r1) / 2.0f;
        if (std::abs(r2 - r1) > std::numbers::pi) { r3 += std::numbers::pi; } //smaller angle

        while (r3 > std::numbers::pi) { r3 -= 2 * std::numbers::pi; } //-pi;pi
        while (r3 < -std::numbers::pi) { r3 += 2 * std::numbers::pi; } //-pi;pi

        if (std::abs(r3) > 1.4 && std::abs(r3) < 1.7) { r3 = 1.5; }//75-105

        r3 += r1;

        /*auto s1 = r1 * 180 / std::numbers::pi;
        auto s2 = r2 * 180 / std::numbers::pi;
        auto s3 = r3 * 180 / std::numbers::pi;
        GameApi::log(
                 INFO << "x " << where.x << " y " << where.y << " | r1: " << r1 << " " << s1 << " | r2: " << r2 << " "
                      << s2 << " | r3: " << r3 << " " << s3);*/
        rigidbody->AddForce({force * cos(r1), force * sin(r1)});
        rigidbody->AddForce({force * cos(r3), force * sin(r3)});
    }
}
