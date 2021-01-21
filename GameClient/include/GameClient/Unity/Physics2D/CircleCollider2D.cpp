//
// Created by Michal_Marszalek on 14.01.2021.
//

#include <box2d/box2d.h>
#include "CircleCollider2D.h"
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Core/Attributes.h>

ADD_COMPONENT_MENU(CircleCollider2D, radius)
ADD_ATTRIBUTE(CircleCollider2D, ExecuteInEditMode)


CircleCollider2D::CircleCollider2D() : radius(this, &CircleCollider2D::Refresh, 1.0f) {

}

void CircleCollider2D::Apply() {
    Destroy();

    b2FixtureDef def;
    b2CircleShape shape;
    shape.m_radius = radius;
    if (!attachedRigidbody) {
        shape.m_p.x = transform()->localPosition().x;
        shape.m_p.y = transform()->localPosition().y;
    }
    shape.m_p.x += offset().x;
    shape.m_p.y += offset().y;

    def.userData = std::static_pointer_cast<Collider2D>(
            static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this());
    def.shape = &shape;

    auto body = GetBody(); //Body of attached body or world body

    fixture = body->CreateFixture(&def);

    RecalculateMass();
}

void CircleCollider2D::Destroy() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }
    RecalculateMass();
}
