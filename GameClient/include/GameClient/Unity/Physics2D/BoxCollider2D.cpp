//
// Created by Michal_Marszalek on 14.01.2021.
//

#include "BoxCollider2D.h"
#include <GameClient/Unity/Serialization/to_json.h>
#include <Editor/Menu.h>
#include <Core/Attributes.h>


ADD_COMPONENT_MENU(BoxCollider2D, size, offset)
ADD_ATTRIBUTE(BoxCollider2D, ExecuteInEditMode)

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
