//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Behaviour.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Transform.h>

EXPORT_CLASS(Behaviour, ("m_enabled", enabled))

Behaviour::Behaviour() : enabled(this, &Behaviour::setEnable, true) {}

bool Behaviour::isActiveAndEnabled() const {
    if (gameObject())
        return enabled && m_gameObjectCacheEnabled;

    return enabled;
}

void Behaviour::UnityOnActiveChange(bool b) {
    m_gameObjectCacheEnabled = b;

    if (b && enabled && !m_en) {
        m_en = true;
        OnEnable();
    }

    if (!(b && enabled) && m_en) {
        m_en = false;
        OnDisable();
    }
}

void Behaviour::setEnable() {
    //Was Awake, Game Object Active and change
    if (!m_unityAwakeed) { return; }

    if (m_gameObjectCacheEnabled && enabled && !m_en) {
        m_en = true;
        OnEnable();
    }

    if (!(m_gameObjectCacheEnabled && enabled) && m_en) {
        m_en = false;
        OnDisable();
    }
}

void Behaviour::UnityUpdate() {
    if (isActiveAndEnabled())
        Update();
}

void Behaviour::UnityLateUpdate() {
    if (isActiveAndEnabled())
        LateUpdate();
}

void Behaviour::UnityFixedUpdate() {
    if (isActiveAndEnabled())
        FixedUpdate();
}
