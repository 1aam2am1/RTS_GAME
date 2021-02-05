//
// Created by Michal_Marszalek on 2021-02-04.
//

#include <numbers>
#include "Bullet.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(Bullet, force)

void Bullet::FixedUpdate() {
    if (body) {
        auto r3 = (transform()->localRotation + 90) * std::numbers::pi / 180.0f;

        body->velocity = sf::Vector2f{static_cast<float>(force * cos(r3)), static_cast<float>(force * sin(r3))};
    }
}

void Bullet::Start() {
    body = GetComponent<Rigidbody2D>();
}
