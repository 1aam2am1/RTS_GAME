//
// Created by Michal_Marszalek on 18.09.2020.
//

#include <GameClient/GlobalStaticVariables.h>
#include "Collider2D.h"

b2Body *Collider2D::GetBody() {
    if (attachedRigidbody) {
        return attachedRigidbody->body;
    }

    return global.physics.worldBody;
}

void Collider2D::Awake() {
    attachedRigidbody = GetComponent<Rigidbody2D>();
}

void Collider2D::RecalculateMass() {
    if (attachedRigidbody) {
        b2MassData mass;
        mass.mass = attachedRigidbody->mass;
        mass.I = attachedRigidbody->inertia;
        attachedRigidbody->body->SetMassData(&mass);
    }
}

void Collider2D::Refresh() {
    if (fixture) {
        Apply();
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
        GameApi::log(ERR.fmt("Dont disable disabled"));
    }
}

void Collider2D::OnDestroy() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
    }
}

Collider2D::~Collider2D() {
    if (fixture) {
        fixture->GetBody()->DestroyFixture(fixture);
        fixture = nullptr;
        GameApi::log(ERR.fmt("Fixture should be destroyed on OnDestroy()"));
    }
}
