//
// Created by Michal_Marszalek on 14.01.2021.
//

#include "BoxCollider2D.h"
#include <GameClient/Unity/Serialization/to_json.h>
#include <Editor/Menu.h>
#include <Core/Attributes.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <numbers>


ADD_COMPONENT_MENU(BoxCollider2D, size)
ADD_ATTRIBUTE(BoxCollider2D, ExecuteInEditMode)

BoxCollider2D::BoxCollider2D() : size(this, &BoxCollider2D::ChangeSize, 1.f, 1.f) {

}

void BoxCollider2D::Apply() {
    Destroy();

    b2FixtureDef def;
    b2PolygonShape shape;
    if (attachedRigidbody) {
        shape.SetAsBox(size().x / 2.f, size().y / 2.f, {offset().x, offset().y}, 0);
        /*b2Vec2 v[3];

        const auto h = 0.5f * std::sqrt(3.0f) / 3.0f;
        v[0] = {-0.5, -h};
        v[1] = {0.5, -h};
        v[2] = {0, 2.0f * h};
        shape.Set(v, 3);*/
    } else {
        shape.SetAsBox(size().x / 2.f, size().y / 2.f,
                       {offset().x + transform()->localPosition().x, offset().y + transform()->localPosition().y},
                       transform()->localRotation() * std::numbers::pi / 180.f);
    }


    def.userData = std::static_pointer_cast<Collider2D>(
            static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this());
    def.shape = &shape;

    auto body = GetBody(); //Body of attached body or world body

    fixture = body->CreateFixture(&def);

    RecalculateMass();
}

void BoxCollider2D::ChangeSize() {
    if (m_unityAwakeed && fixture)
        Apply();
}

void BoxCollider2D::Destroy() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }
    RecalculateMass();
}
