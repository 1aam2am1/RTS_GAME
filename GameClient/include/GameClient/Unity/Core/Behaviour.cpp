//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Behaviour.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(Behaviour, ("m_enabled", enabled))

Behaviour::Behaviour() : enabled(true) {}

bool Behaviour::isActiveAndEnabled() const {
    return enabled && gameObject()->activeInHierarchy();
}

void Behaviour::UnityOnActiveChange(bool b) {
    if (b && enabled && !m_onEnable) {
        m_onEnable = true;
        OnEnable();
    }

    if (!(b && enabled) && m_onEnable) {
        m_onEnable = false;
        OnDisable();
    }
}
