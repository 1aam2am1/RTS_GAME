//
// Created by Michal_Marszalek on 2021-01-24.
//

#include "Placer.h"
#include <imgui.h>
#include <GameClient/Unity/Core/Camera.h>

void Placer::OnGUI() {
    const bool is_hovered = ImGui::IsWindowHovered(); // Hovered

    if (ImGui::IsMouseClicked(0) && is_hovered) {
        for (auto &&c : GetComponents<MonoBehaviour>()) {
            c->enabled = true;
        }
        Destroy(shared_from_this());
    }

    transform()->localPosition = Camera::main()->transform()->localPosition();
}
