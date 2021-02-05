//
// Created by Michal_Marszalek on 2021-01-24.
//

#include "Placer.h"
#include <imgui.h>
#include <GameClient/Unity/Core/Camera.h>
#include <Physics2D/Physics2D.h>

void Placer::OnGUI() {
    const bool is_hovered = ImGui::IsWindowHovered(); // Hovered

    if (ImGui::IsMouseClicked(0) && is_hovered) {
        auto box = Physics2D::OverlapBoxAll({transform()->localPosition().x, transform()->localPosition().y}, {1, 1});
        bool produce = true;

        for (auto &&b : box) {
            if ((!b->attachedRigidbody || b->attachedRigidbody->bodyType() != RigidbodyType2D::Dynamic) &&
                b->gameObject() != gameObject()) {
                produce = false;
                break;
            }
        }

        if (produce) {
            for (auto &&c : GetComponents<MonoBehaviour>()) {
                c->enabled = true;
            }
            Destroy(shared_from_this());
        }
    }

    transform()->localPosition = Camera::main()->transform()->localPosition();
}
