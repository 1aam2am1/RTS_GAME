//
// Created by Michal_Marszalek on 14.01.2021.
//

#include <Macro.h>
#include <GameClient/GlobalStaticVariables.h>
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Physics2D/Collider2D.h>
#include <numbers>

ADD_COMPONENT_MENU(Rigidbody2D, bodyType, mass, inertia)

Rigidbody2D::~Rigidbody2D() {
    auto list = body->GetFixtureList();
    bool active = gameObject()->activeInHierarchy();
    while (list) {
        auto collider = dynamic_pointer_cast<Collider2D>(list->GetUserData().lock());
        if (collider && active) {
            collider->Apply(); //RigidBody is destroyed
        } else {
            body->DestroyFixture(list); //GameObject is destroyed
        }

        list = body->GetFixtureList();
    }
}

void Rigidbody2D::Awake() {
    auto p = transform()->localPosition.get();
    b2BodyDef def;
    def.position = {p.x, p.y};
    def.angle = transform()->localRotation * std::numbers::pi / 180.f;
    def.userData = static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this();
    def.type = static_cast<b2BodyType>(bodyType);

    body = global.physics.world.CreateBody(&def);
}
