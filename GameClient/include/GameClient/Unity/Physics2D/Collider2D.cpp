//
// Created by Michal_Marszalek on 18.09.2020.
//

#include <GameClient/GlobalStaticVariables.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include "Collider2D.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(Collider2D, offset)

Collider2D::Collider2D() : offset(this, &Collider2D::Refresh, 0.f, 0.f) {

}

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

void Collider2D::Refresh() {
    if (isActiveAndEnabled() && transform()) {
        Apply();
    } else if (!transform()) {
        Destroy();
    }
}

void Collider2D::OnEnable() {
    Apply();
}

void Collider2D::OnDisable() {
    Destroy();
}

void Collider2D::OnDestroy() {
    Destroy();
    if (auto t = transform()) { t->m_colliders--; }
}

void Collider2D::RecalculateMass() {
    if (attachedRigidbody)
        attachedRigidbody->RecalculateMass();
}

Collider2D::~Collider2D() {

}
