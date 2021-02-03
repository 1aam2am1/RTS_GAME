//
// Created by Michal_Marszalek on 2021-02-02.
//

#include "Attack.h"
#include "Life.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(Attack, attack)

void Attack::OnCollisionEnter2D(const Collision2D &other) {
    //selective attack
    if (other.gameObject->layer != gameObject()->layer) {
        auto life = other.gameObject->GetComponent<Life>();
        if (life)
            life->attacked(attack);
    }

    Destroy(gameObject());
}
