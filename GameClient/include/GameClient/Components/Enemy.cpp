//
// Created by Michal_Marszalek on 2021-01-22.
//

#include "Enemy.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>

ADD_USER_COMPONENT(Enemy)

void Enemy::Update() {
    if (other_enemy == nullptr) {
        GameApi::log(CRIT << "Someone have won");
        Application::Quit();
    }

    /// TODO: Ai min-max

}
