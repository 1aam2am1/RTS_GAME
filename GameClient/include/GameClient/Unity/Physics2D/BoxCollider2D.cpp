//
// Created by Michal_Marszalek on 14.01.2021.
//

#include "BoxCollider2D.h"
#include <Serialization/to_json.h>
#include <Editor/Menu.h>


ADD_COMPONENT_MENU(BoxCollider2D, size, offset)

void BoxCollider2D::Apply() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }

    b2FixtureDef def;
    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2.f, size.y / 2.f, {offset.x, offset.y}, 0);

    def.userData = static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this();
    def.shape = &shape;

    auto body = GetBody(); //Body of attached body or world body

    fixture = body->CreateFixture(&def);

    RecalculateMass();

}
