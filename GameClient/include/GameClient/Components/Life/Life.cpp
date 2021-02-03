//
// Created by Michal_Marszalek on 2021-02-02.
//

#include "Life.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(Life, life)

void Life::attacked(float i) {
    life -= i;

    if (life <= 0) {
        Destroy(gameObject());
    }
}
