//
// Created by Michal_Marszalek on 2021-02-04.
//

#include "Pause.h"
#include <imgui.h>
#include <SFML/Window/Keyboard.hpp>
#include <Core/Time.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(Pause)


void Pause::OnGUI() {
    if (ImGui::IsKeyReleased(sf::Keyboard::Escape)) {
        if (Time::timeScale >= 0.5) {
            Time::timeScale = 0;
        } else {
            Time::timeScale = 1;
        }
    }

    if (Time::timeScale < 0.1f) {
        ImGui::SetWindowFontScale(10);
        ImGui::Text("PAUSE");
        ImGui::SetWindowFontScale(1);
    }
}
