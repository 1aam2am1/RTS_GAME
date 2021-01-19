//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <imgui.h>
#include <GameClient/GlobalStaticVariables.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>
#include "GameSceneMenuGizmo.h"

void DrawGameSceneGizmoMenu() {
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
