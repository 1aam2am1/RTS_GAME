//
// Created by Michal_Marszalek on 2021-01-22.
//

#include <Core/Application.h>
#include "AiEnemy.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(AiEnemy)

void AiEnemy::Update() {
    if (other_enemy == nullptr) {
        GameApi::log(CRIT << "Someone have won");
        Application::Quit();
    }

    /// TODO: Ai min-max

}
