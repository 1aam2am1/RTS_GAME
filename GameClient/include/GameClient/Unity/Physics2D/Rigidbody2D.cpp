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
#include <Core/Attributes.h>

ADD_COMPONENT_MENU(Rigidbody2D, bodyType, mass, inertia)
ADD_ATTRIBUTE(Rigidbody2D, ExecuteInEditMode)

Rigidbody2D::~Rigidbody2D() {
    if (!body) { return; }
    auto list = body->GetFixtureList();
    while (list) {
        auto collider = dynamic_pointer_cast<Collider2D>(list->GetUserData().lock());
        if (collider) {
            collider->Refresh(); //RigidBody is destroyed
        }

        list = body->GetFixtureList();
    }

    body->GetWorld()->DestroyBody(body);
}

void Rigidbody2D::Awake() {
    auto p = transform()->localPosition.get();
    b2BodyDef def;
    def.position = {p.x, p.y};
    def.angle = transform()->localRotation * std::numbers::pi / 180.f;
    def.userData = static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this();
    def.type = static_cast<b2BodyType>(bodyType);
    def.enabled = false;

    body = global.physics.world.CreateBody(&def);

    auto colliders = gameObject()->GetComponents<Collider2D>();
    for (auto &c : colliders) {
        if (!c->attachedRigidbody) {
            c->attachedRigidbody = static_pointer_cast<Rigidbody2D>(shared_from_this());
            c->Refresh();
        }
    }
}

void Rigidbody2D::UnityOnActiveChange(bool b) {
    if (!body) { return; }

    body->SetEnabled(b);
}
