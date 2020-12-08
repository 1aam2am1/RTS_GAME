//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Behaviour.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(Behaviour, enabled)

bool Behaviour::isActiveAndEnabled() const {
    return enabled && gameObject()->activeInHierarchy();
}
