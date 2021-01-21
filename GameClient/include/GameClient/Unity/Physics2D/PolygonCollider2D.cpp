//
// Created by Michal_Marszalek on 14.01.2021.
//

#include <box2d/b2_contact.h>
#include <box2d/box2d.h>
#include <Macro.h>
#include "PolygonCollider2D.h"
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <Core/Attributes.h>
#include <numbers>

ADD_COMPONENT_MENU(PolygonCollider2D, paths)
ADD_ATTRIBUTE(PolygonCollider2D, ExecuteInEditMode)

void PolygonCollider2D::Apply() {
    Destroy();

    for (auto &m : paths) {
        b2FixtureDef def;
        b2PolygonShape shape;
        try {
            b2Vec2 points[b2_maxPolygonVertices];
            if (attachedRigidbody) {
                for (uint32_t i = 0; i < m.size(); ++i) {
                    points[i].x = m[i].x + offset().x;
                    points[i].y = m[i].y + offset().y;
                }
            } else {
                for (uint32_t i = 0; i < m.size(); ++i) {
                    auto angle = transform()->localRotation() * std::numbers::pi / 180.f;
                    auto c = std::cos(angle);
                    auto s = std::sin(angle);
                    float xnew = m[i].x * c - m[i].y * s;
                    float ynew = m[i].x * s + m[i].y * c;
                    points[i].x = xnew + offset().x + transform()->localPosition().x;
                    points[i].y = ynew + offset().y + transform()->localPosition().y;
                }
            }

            shape.Set(points, m.size() < b2_maxPolygonVertices ? m.size() : b2_maxPolygonVertices);
        } EXCEPTION_PRINT

        def.userData = std::static_pointer_cast<Collider2D>(
                static_cast<std::enable_shared_from_this<Object> *>(this)->shared_from_this());
        def.shape = &shape;

        auto body = GetBody(); //Body of attached body or world body

        auto fixture = body->CreateFixture(&def);
        fixtures.emplace_back(fixture);
    }

    RecalculateMass();
}

void PolygonCollider2D::Destroy() {
    for (auto &f : fixtures) {
        f->GetBody()->DestroyFixture(f);
    }
    fixtures.clear();
    RecalculateMass();
}

int PolygonCollider2D::pathCount() {
    return paths.size();
}

void PolygonCollider2D::SetPath(int index, const std::vector<sf::Vector2f> &points) {
    if (index < pathCount()) {
        paths[index] = points;
    } else if (index == pathCount()) {
        paths.emplace_back(points);
    } else {
        throw std::runtime_error("Index is too big");
    }
}

std::vector<sf::Vector2f> PolygonCollider2D::GetPath(int index) {
    if (index < pathCount()) {
        return paths[index];
    }

    return {};
}
