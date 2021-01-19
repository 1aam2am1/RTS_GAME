//
// Created by Michal_Marszalek on 18.09.2020.
//

#include <GameClient/GlobalStaticVariables.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include "Collider2D.h"

b2Body *Collider2D::GetBody() {
    if (attachedRigidbody) {
        return attachedRigidbody->body;
    }

    return global.physics.worldBody;
}

void Collider2D::Awake() {
    attachedRigidbody = GetComponent<Rigidbody2D>();
    transform()->m_colliders++;
}

void Collider2D::RecalculateMass() {
    if (attachedRigidbody) {
        b2MassData mass;
        mass.mass = attachedRigidbody->mass;
        mass.center = {0, 0};
        mass.I = attachedRigidbody->inertia;
        attachedRigidbody->body->SetMassData(&mass);
    }
}

void Collider2D::Refresh() {
    if (fixture && transform()) {
        Apply();
    } else if (!transform()) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }
}

void Collider2D::OnEnable() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;

        GameApi::log(ERR.fmt("Dont enable enabled"));
    }
    Apply();
}

void Collider2D::OnDisable() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;

    } else {
        if (transform()) //transform() == nullptr => Destroy on Refresh from Rigidbody2D
            GameApi::log(ERR.fmt("Dont disable disabled"));
    }
}

void Collider2D::OnDestroy() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (auto t = transform()) { t->m_colliders--; }
}

Collider2D::~Collider2D() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
        GameApi::log(ERR.fmt("Fixture should be destroyed on OnDestroy()"));
    }
}
