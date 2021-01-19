//
// Created by Michal_Marszalek on 14.01.2021.
//

#include <Macro.h>
#include <GameClient/GlobalStaticVariables.h>
#include "Rigidbody2D.h"
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Physics2D/Collider2D.h>
#include <numbers>
#include <Core/Attributes.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

ADD_COMPONENT_MENU(Rigidbody2D, bodyType, mass, inertia)
ADD_ATTRIBUTE(Rigidbody2D, ExecuteInEditMode, DisallowMultipleComponent)

Rigidbody2D::Rigidbody2D() : bodyType(this, &Rigidbody2D::ChangeBody, RigidbodyType2D::Dynamic),
                             mass(this, &Rigidbody2D::ChangeBody, 1.f),
                             inertia(this, &Rigidbody2D::ChangeBody, 0.1f) {}

Rigidbody2D::~Rigidbody2D() {
    if (!body) { return; }
    if (auto t = transform()) { t->m_physics_root = nullptr; }

    auto list = body->GetFixtureList();
    while (list) {
        auto collider = dynamic_pointer_cast<Collider2D>(list->GetUserData().lock());
        if (collider) {
            collider->Refresh(); //RigidBody is destroyed
        }

        list = list->GetNext();
    }

    body->GetWorld()->DestroyBody(body);
}

void Rigidbody2D::Awake() {
    auto p = transform()->localPosition.get();
    b2BodyDef def;
    def.position = {p.x, p.y};
    def.angle = transform()->localRotation * std::numbers::pi / 180.f;
    def.userData = static_pointer_cast<Transform>(
            static_cast<std::enable_shared_from_this<Object> *>(transform().get())->shared_from_this());
    def.type = static_cast<b2BodyType>(bodyType());
    def.enabled = true;
    def.fixedRotation = false;

    body = global.physics.world.CreateBody(&def);

    if (!transform()->m_physics_root) {
        transform()->m_physics_root = body;
    } else {
        assert(0); // Physical body for this transform is used up
    }


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

void Rigidbody2D::ChangeBody() {
    if (inertia < 0.f) { inertia.t = 0.f; }
    if (body) {
        b2MassData m;
        m.mass = mass;
        m.center = {0, 0};
        m.I = inertia;
        body->SetMassData(&m);
    }
}
