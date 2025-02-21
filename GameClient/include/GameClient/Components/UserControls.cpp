//
// Created by Michal_Marszalek on 2021-01-22.
//

#include "UserControls.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include "Placer.h"
#include "PrefabFunc.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <Core/Camera.h>
#include <GameClient/Unity/Core/SpriteRenderer.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <Physics2D/BoxCollider2D.h>
#include <Core/Attributes.h>
#include "Building.h"

ADD_USER_COMPONENT(UserControls, upper)
ADD_ATTRIBUTE(UserControls, DontNetworkSynchronize)


void UserControls::Start() {
    camera = FindObjectOfType<Camera>()->transform();
    base = GetComponent<Enemy>();

    if(!base){Destroy(gameObject());}
}

void UserControls::OnGUI() {
    const auto window = ImGui::GetCurrentWindow();
    const auto size = window->ContentRegionRect.GetSize();

    ImGui::PushStyleColor(ImGuiCol_Text, ImColor(1.0f, 0.f, 0.f).Value);

    ImGui::SetCursorPos({50, upper});
    ImGui::BeginChild("###res", {120, 90});
    ImGui::Text("Resources");

    ImGui::Text("Water:  %.1f", base->resources[ResourceType::water]);
    ImGui::Text("Metal:  %.1f", base->resources[ResourceType::metal]);
    ImGui::Text("Food:   %.1f ", base->resources[ResourceType::food]);

    ImGui::EndChild();

    ImGui::SetCursorPos({size.x - 50 - 120, upper});
    ImGui::BeginChild("###prio", {120, 90});

    ImGui::Text("Water: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderFloat("##w", &(base->needed[ResourceType::water]), 0.f, 1.f);

    ImGui::Text("Metal: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderFloat("##m", &(base->needed[ResourceType::metal]), 0.f, 1.f);

    ImGui::Text("Food:  ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::SliderFloat("##f", &(base->needed[ResourceType::food]), 0.f, 1.f);

    ImGui::EndChild();

    {
        std::string key;

        switch (base->cell) {
            case mono_state::wait:
                key = "  Wait  ";
                break;
            case mono_state::flee:
                key = "  Flee  ";
                break;
            case mono_state::attack:
                key = " Attack ";
                break;
            case mono_state::resource:
                key = "Resource";
                break;
        }

        key += "###cell";

        auto button_size = ImGui::CalcTextSize(key.data()).x + ImGui::GetStyle().FramePadding.x * 2.0f;

        ImGui::SetCursorPos({(size.x - button_size) / 2.0f, upper});
        if (ImGui::ButtonEx(key.data(), {0, 0}, ImGuiButtonFlags_AllowItemOverlap)) {
            base->cell = static_cast<mono_state>(((int) base->cell() + 1) % 4);
        }
    }


    ImGui::SetCursorPos({upper, size.y + window->ContentRegionRect.Min.y - upper - 90});
    ImGui::BeginChild("###create", {size.x - 2.0f * upper, 120});

    float button_size_x = 200;
    if ((size.x - 2.0f * upper) < 400) {
        button_size_x = (size.x - 2.0f * upper) / 2.0f;
    }

    auto count = std::count_if(base->ship_production.begin(), base->ship_production.end(),
                               [](auto &&b) { return b == ShipType::Attack; });
    if (ImGui::Button(("Attack Ship (" + GameApi::to_string(count) + ")\n(0, 200, 100)").data(),
                      {button_size_x, 45})) {
        base->ProduceShip(ShipType::Attack);
        count = std::count_if(base->ship_production.begin(), base->ship_production.end(),
                              [](auto &&b) { return b == ShipType::Attack; });
    }

    ImGui::SameLine();

    if (ImGui::Button(
            ("Resource Ship (" + GameApi::to_string(base->ship_production.size() - count) + ")\n(0, 100, 100)").data(),
            {button_size_x, 45})) {
        base->ProduceShip(ShipType::Resource);
    }


    if (ImGui::Button("Attack Shipyard\n(1000, 600, 500)", {button_size_x, 45})) {
        createBuildingPlacer(ShipType::Attack);
    }

    ImGui::SameLine();

    if (ImGui::Button("Resource Shipyard\n(1000, 300, 500)", {button_size_x, 45})) {
        createBuildingPlacer(ShipType::Resource);
    }

    ImGui::EndChild();

    auto text_size = ImGui::CalcTextSize("X: -300 Y: +300");
    ImGui::SetCursorPos({ImGui::GetStyle().FramePadding.x,
                         ImGui::GetWindowContentRegionMax().y - text_size.y - ImGui::GetStyle().FramePadding.y});
    ImGui::Text("X: %.1f Y: %.1f", camera->localPosition().x, camera->localPosition().y);

    ImGui::PopStyleColor();
}

void UserControls::createBuildingPlacer(ShipType type) {
    auto go = base->ProduceBuilding(type);
    if (!go) { return; }
    go->GetComponent<Building>()->enabled = false;
    go->AddComponent<Placer>();

}
