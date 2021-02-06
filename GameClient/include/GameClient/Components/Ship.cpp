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
    } else {
        parent->AddShip(shared_from_this());
        gameObject()->layer = parent->gameObject()->layer;
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
        auto r1 = std::atan2(where.y, where.x);
        auto r2 = std::atan2(other.normal.y, other.normal.x);
        auto r3 = (r2 - r1) / 2.0f;
        if (std::abs(r2 - r1) > std::numbers::pi) { r3 += std::numbers::pi; } //smaller angle

        while (r3 > std::numbers::pi) { r3 -= 2 * std::numbers::pi; } //-pi;pi
        while (r3 < -std::numbers::pi) { r3 += 2 * std::numbers::pi; } //-pi;pi

        if (std::abs(r3) > 1.3 && std::abs(r3) < 1.8) { r3 = 1.5; }//75-105

        r3 += r1;

        /*auto s1 = r1 * 180 / std::numbers::pi;
        auto s2 = r2 * 180 / std::numbers::pi;
        auto s3 = r3 * 180 / std::numbers::pi;
        GameApi::log(
                 INFO << "x " << where.x << " y " << where.y << " | r1: " << r1 << " " << s1 << " | r2: " << r2 << " "
                      << s2 << " | r3: " << r3 << " " << s3);*/
        /*
        auto w = target->localPosition() - transform()->localPosition();
        auto r1 = atan2(w.y, w.x);
        auto r2 = atan2(other.normal.y, other.normal.x);
        auto r3 = (r2 - r1) / 2.0f;
        if (std::abs(r3) > 1.3 && std::abs(r3) < 1.8) { r3 = 1.5; }
        r3 += r1;

        if (std::abs(r2 - r1) > std::numbers::pi) { r3 += std::numbers::pi; } //smaller angle
*/
        rigidbody->AddForce({force * cos(r3), force * sin(r3)});
    }
}
