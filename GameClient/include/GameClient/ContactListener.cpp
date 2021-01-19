//
// Created by Michal_Marszalek on 2021-01-18.
//

#include "ContactListener.h"
#include <GameClient/Unity/Core/MonoBehaviour.h>
#include <GameClient/TPtr.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>

auto to_collision = [](b2Contact *o) {
    std::pair<Collision2D, Collision2D> p;
    //A
    p.first.collider = o->GetFixtureB()->GetUserData().lock();
    p.first.otherCollider = o->GetFixtureA()->GetUserData().lock();
    //B
    p.second.collider = p.first.otherCollider;
    p.second.otherCollider = p.first.collider;


    //Inverse
    p.first.transform = o->GetFixtureB()->GetBody()->GetUserData().lock();
    p.second.transform = o->GetFixtureA()->GetBody()->GetUserData().lock();

    if (!p.first.transform) { p.first.transform = p.first.collider->transform(); } //Is World?
    if (!p.second.transform) { p.second.transform = p.second.collider->transform(); } //Is World?

    //Inverse
    p.first.gameObject = !p.first.transform.expired() ? p.first.transform->gameObject() : TPtr<GameObject>{};
    p.second.gameObject = !p.second.transform.expired() ? p.second.transform->gameObject() : TPtr<GameObject>{};


    //A
    p.first.rigidbody = !p.first.collider.expired() ? p.first.collider->attachedRigidbody : TPtr<Rigidbody2D>();
    p.first.otherRigidbody = !p.first.otherCollider.expired() ? p.first.otherCollider->attachedRigidbody
                                                              : TPtr<Rigidbody2D>();

    p.first.relativeVelocity.x =
            o->GetFixtureB()->GetBody()->GetLinearVelocity().x - o->GetFixtureA()->GetBody()->GetLinearVelocity().x;
    p.first.relativeVelocity.y =
            o->GetFixtureB()->GetBody()->GetLinearVelocity().y - o->GetFixtureA()->GetBody()->GetLinearVelocity().y;

    //B
    p.second.rigidbody = p.first.otherRigidbody;
    p.second.otherRigidbody = p.first.rigidbody;
    p.second.relativeVelocity = -p.first.relativeVelocity;

    return p;
};

void ContactListener::BeginContact(b2Contact *o) {
    if (o->GetFixtureA()->IsSensor() || o->GetFixtureB()->IsSensor()) {
        if (o->GetFixtureA()->IsSensor() && o->GetFixtureB()->IsSensor()) { return; }

        auto fixture_sensor = o->GetFixtureA()->IsSensor() ? o->GetFixtureA() : o->GetFixtureB();
        auto fixture_collider = o->GetFixtureA()->IsSensor() ? o->GetFixtureB() : o->GetFixtureA();

        auto transform_sensor = fixture_sensor->GetBody()->GetUserData().lock();

        if (!transform_sensor) { return; }
        std::pair<TPtr<GameObject>, TPtr<Collider2D>> p;

        p.first = transform_sensor->gameObject(); //Sensor game object
        p.second = fixture_collider->GetUserData().lock();

        m_trigger_new.emplace_back(p);
        to_trigger_stay.emplace(o);
    } else {
        auto col = to_collision(o);

        m_new.emplace_back(col);
        to_stay.emplace(o);
    }
}

void ContactListener::EndContact(b2Contact *o) {
    if (to_trigger_stay.erase(o) || m_trigger_stays.erase(o)) {
        if (o->GetFixtureA()->IsSensor() && o->GetFixtureB()->IsSensor()) { return; }

        auto fixture_sensor = o->GetFixtureA()->IsSensor() ? o->GetFixtureA() : o->GetFixtureB();
        auto fixture_collider = o->GetFixtureA()->IsSensor() ? o->GetFixtureB() : o->GetFixtureA();

        auto transform_sensor = fixture_sensor->GetBody()->GetUserData().lock();

        if (!transform_sensor) { return; }
        std::pair<TPtr<GameObject>, TPtr<Collider2D>> p;

        p.first = transform_sensor->gameObject(); //Sensor game object
        p.second = fixture_collider->GetUserData().lock();

        m_trigger_exit.emplace_back(p);
    } else if (to_stay.erase(o) || m_stays.erase(o)) {
        auto col = to_collision(o);

        m_exit.emplace_back(col);
    } else {
        if (o->GetFixtureA()->IsSensor() && o->GetFixtureB()->IsSensor()) { return; }
        if (o->GetFixtureA()->IsSensor() || o->GetFixtureB()->IsSensor()) {
            auto fixture_sensor = o->GetFixtureA()->IsSensor() ? o->GetFixtureA() : o->GetFixtureB();

            auto transform_sensor = fixture_sensor->GetBody()->GetUserData().lock();

            if (!transform_sensor) { return; }
        }

        GameApi::log(ERR.fmt("Exit not on contracted is wrong"));
    }

}

void ContactListener::Update() {
    for (auto &&o : to_stay) {
        m_stays.emplace(o, to_collision(o));
    }
    to_stay.clear();

    for (auto &&o : to_trigger_stay) {
        std::pair<TPtr<GameObject>, TPtr<Collider2D>> p;

        if (o->GetFixtureA()->IsSensor() && o->GetFixtureB()->IsSensor()) { continue; }
        auto fixture_sensor = o->GetFixtureA()->IsSensor() ? o->GetFixtureA() : o->GetFixtureB();
        auto fixture_collider = o->GetFixtureA()->IsSensor() ? o->GetFixtureB() : o->GetFixtureA();

        auto transform_sensor = fixture_sensor->GetBody()->GetUserData().lock();

        if (!transform_sensor) { continue; }
        p.first = transform_sensor->gameObject(); //Sensor game object

        p.second = fixture_collider->GetUserData().lock();

        m_trigger_stays.emplace(o, p);
    }
    to_trigger_stay.clear();
}

void ContactListener::Call() {

    for (auto &&o : m_stays) {
        o.second.first.relativeVelocity.x =
                o.first->GetFixtureB()->GetBody()->GetLinearVelocity().x -
                o.first->GetFixtureA()->GetBody()->GetLinearVelocity().x;
        o.second.first.relativeVelocity.y =
                o.first->GetFixtureB()->GetBody()->GetLinearVelocity().y -
                o.first->GetFixtureA()->GetBody()->GetLinearVelocity().y;

        o.second.second.relativeVelocity = -o.second.first.relativeVelocity;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    auto call_trigger = [](auto &vec, auto f) {
        std::decay_t<decltype(vec)> copy;
        std::swap(copy, vec);

        for (auto &&o : copy) {
            auto mono = o.first->template GetComponents<MonoBehaviour>();
            if (o.second) {
                for (auto &&m : mono) {
                    if (m) {
                        (m.get()->*f)(o.second);
                    }
                }
            }
        }
    };
    call_trigger(m_trigger_new, &MonoBehaviour::OnTriggerEnter2D);
    for (auto &&o : m_trigger_stays) {
        auto mono = o.second.first->GetComponents<MonoBehaviour>();
        if (o.second.second) {
            for (auto &&m : mono) {
                if (m) {
                    m->OnTriggerStay2D(o.second.second);
                }
            }
        }
    }
    call_trigger(m_trigger_exit, &MonoBehaviour::OnTriggerExit2D);

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    auto call_collision = [](auto &vec, auto f) {
        std::decay_t<decltype(vec)> copy;
        std::swap(copy, vec);

        for (auto &&o : copy) {
            if (o.first.gameObject) {
                auto mono = o.first.gameObject->template GetComponents<MonoBehaviour>();
                for (auto &&m : mono) {
                    if (m) {
                        (m.get()->*f)(o.second);
                    }
                }
            }
            if (o.second.gameObject) {
                auto mono = o.second.gameObject->template GetComponents<MonoBehaviour>();
                for (auto &&m : mono) {
                    if (m) {
                        (m.get()->*f)(o.first);
                    }
                }
            }
        }

    };

    call_collision(m_new, &MonoBehaviour::OnCollisionEnter2D);
    for (auto &&o : m_stays) {
        if (o.second.first.gameObject) {
            auto mono = o.second.first.gameObject->GetComponents<MonoBehaviour>();
            for (auto &&m : mono) {
                if (m) {
                    m->OnCollisionStay2D(o.second.second);
                }
            }
        }
        if (o.second.second.gameObject) {
            auto mono = o.second.second.gameObject->GetComponents<MonoBehaviour>();
            for (auto &&m : mono) {
                if (m) {
                    m->OnCollisionStay2D(o.second.first);
                }
            }
        }
    }
    call_collision(m_exit, &MonoBehaviour::OnCollisionExit2D);
}

bool ContactListener::empty() {
    return m_stays.empty() && to_stay.empty() && m_trigger_stays.empty() && to_trigger_stay.empty();
}
