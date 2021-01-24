//
// Created by Michal_Marszalek on 2021-01-24.
//

#include <GameClient/GlobalStaticVariables.h>
#include "Physics2D.h"
#include <box2d/box2d.h>

std::vector<TPtr<Collider2D>> Physics2D::OverlapBoxAll(sf::Vector2f point, sf::Vector2f size) {
    struct R : b2QueryCallback {
        bool ReportFixture(b2Fixture *fixture) override {

            auto collider2d = dynamic_pointer_cast<Collider2D>(fixture->GetUserData().lock());
            if (collider2d) {
                result.emplace_back(collider2d);
            }

            return true;
        }

        std::vector<TPtr<Collider2D>> result;
    };

    R r;
    b2AABB v;

    v.lowerBound.x = point.x - (size.x / 2.0f);
    v.lowerBound.y = point.y - (size.y / 2.0f);

    v.upperBound.x = point.x + (size.x / 2.0f);
    v.upperBound.y = point.y + (size.y / 2.0f);

    global.physics.world.QueryAABB(&r, v);

    return r.result;
}

TPtr<Collider2D> Physics2D::OverlapPoint(sf::Vector2f point) {
    struct R : b2QueryCallback {
        b2Vec2 m_point;

        bool ReportFixture(b2Fixture *fixture) override {
            bool inside = fixture->TestPoint(m_point);
            if (inside) {
                auto collider2d = dynamic_pointer_cast<Collider2D>(fixture->GetUserData().lock());
                if (collider2d) {
                    result = collider2d;
                    return false;
                }
            }


            return true;
        }

        TPtr<Collider2D> result;
    };

    R r;
    r.m_point.Set(point.x, point.y);
    b2AABB v;

    v.lowerBound.x = point.x - 0.001f;
    v.lowerBound.y = point.y - 0.001f;

    v.upperBound.x = point.x + 0.001f;
    v.upperBound.y = point.y + 0.001f;

    global.physics.world.QueryAABB(&r, v);

    return r.result;
}

std::vector<TPtr<Collider2D>> Physics2D::OverlapCircleAll(sf::Vector2f point, float radius) {
    struct R : b2QueryCallback {
        b2Vec2 point;
        float radius;

        bool ReportFixture(b2Fixture *fixture) override {
            //fixture->GetShape()->
            //fixture->GetShape()->
            //b2TestOverlap()
            //TODO: Test for circle overlapping, and not center body

            auto collider2d = dynamic_pointer_cast<Collider2D>(fixture->GetUserData().lock());
            if (collider2d) {
                auto position = collider2d->transform()->localPosition();
                b2Vec2 l = point - b2Vec2{position.x, position.y};

                if (l.LengthSquared() < radius * radius) {
                    result.emplace_back(collider2d);
                }
            }

            return true;
        }

        std::vector<TPtr<Collider2D>> result;
    };

    R r;
    b2AABB v;

    r.point.Set(point.x, point.y);
    r.radius = radius;
    v.lowerBound.x = point.x - radius;
    v.lowerBound.y = point.y - radius;

    v.upperBound.x = point.x + radius;
    v.upperBound.y = point.y + radius;

    global.physics.world.QueryAABB(&r, v);

    return r.result;
}
