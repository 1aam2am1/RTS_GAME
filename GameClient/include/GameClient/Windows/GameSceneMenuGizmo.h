//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_GAMESCENEMENUGIZMO_H
#define RTS_GAME_GAMESCENEMENUGIZMO_H

#include <imgui.h>
#include <GameClient/GlobalStaticVariables.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>
#include "GameSceneMenuGizmo.h"

inline void DrawGameSceneGizmoMenu() {
    auto enabled_push = []() {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
    };

    auto enabled_pop = []() {
        ImGui::PopStyleColor();
    };


    if (ImGui::BeginMenuBar()) {
        bool enabled = global.mis.draw_gizmo;

        if (enabled) { enabled_push(); }

        ImGui::SetCursorPosX(0);
        if (ImGui::Button("Draw gizmos")) { global.mis.draw_gizmo = !global.mis.draw_gizmo; }

        if (enabled) { enabled_pop(); }

        ImGui::EndMenuBar();
    }
}

#endif //RTS_GAME_GAMESCENEMENUGIZMO_H
