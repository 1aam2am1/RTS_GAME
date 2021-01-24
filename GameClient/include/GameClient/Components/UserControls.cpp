//
// Created by Michal_Marszalek on 2021-01-22.
//

#include <Core/MonoBehaviour.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include "Enemy.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <Core/Camera.h>


class UserControls : public Enemy {
public:

    float upper = 90.0f;
    TPtr<Transform> camera;

    void Start() override {
        Enemy::Start();
        camera = FindObjectOfType<Camera>()->transform();
    }

    void Update() override {}

    void OnGUI() override {
        const auto window = ImGui::GetCurrentWindow();
        const auto size = window->ContentRegionRect.GetSize();

        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(1.0f, 0.f, 0.f).Value);

        ImGui::SetCursorPos({50, upper});
        ImGui::BeginChild("###res", {120, 90});
        ImGui::Text("Resources");
        if (baze) {
            ImGui::Text("Water:  %.1f", baze->resources[ResourceType::water]);
            ImGui::Text("Metal:  %.1f", baze->resources[ResourceType::metal]);
            ImGui::Text("Food:   %.1f ", baze->resources[ResourceType::food]);
        }
        ImGui::EndChild();

        ImGui::SetCursorPos({size.x - 50 - 120, upper});
        ImGui::BeginChild("###prio", {120, 90});

        ImGui::Text("Water: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderFloat("##w", &(needed[ResourceType::water]), 0.f, 1.f);

        ImGui::Text("Metal: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderFloat("##m", &(needed[ResourceType::metal]), 0.f, 1.f);

        ImGui::Text("Food:  ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderFloat("##f", &(needed[ResourceType::food]), 0.f, 1.f);

        ImGui::EndChild();

        {
            std::string key;

            switch (cell) {
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
                cell = static_cast<mono_state>(((int) cell + 1) % 4);
            }
        }


        ImGui::SetCursorPos({upper, size.y + window->ContentRegionRect.Min.y - upper - 90});
        ImGui::BeginChild("###create", {size.x - 2.0f * upper, 120});

        float button_size_x = 200;
        if ((size.x - 2.0f * upper) < 400) {
            button_size_x = (size.x - 2.0f * upper) / 2.0f;
        }

        auto count = std::count_if(ship_production.begin(), ship_production.end(),
                                   [](auto &&b) { return b == ShipType::Attack; });
        if (ImGui::Button(("Attack Ship (" + GameApi::to_string(count) + ")").data(), {button_size_x, 45})) {
            ProduceShip(ShipType::Attack);
            count = std::count_if(ship_production.begin(), ship_production.end(),
                                  [](auto &&b) { return b == ShipType::Attack; });
        }

        ImGui::SameLine();

        if (ImGui::Button(("Resource Ship (" + GameApi::to_string(ship_production.size() - count) + ")").data(),
                          {button_size_x, 45})) {
            ProduceShip(ShipType::Resource);
        }


        if (ImGui::Button("Attack Shipyard", {button_size_x, 45})) {
            createBuildingPlacer(ShipType::Attack);
        }

        ImGui::SameLine();

        if (ImGui::Button("Resource Shipyard", {button_size_x, 45})) {
            createBuildingPlacer(ShipType::Resource);
        }

        ImGui::EndChild();

        auto text_size = ImGui::CalcTextSize("X: -300 Y: +300");
        ImGui::SetCursorPos({ImGui::GetStyle().FramePadding.x,
                             ImGui::GetWindowContentRegionMax().y - text_size.y - ImGui::GetStyle().FramePadding.y});
        ImGui::Text("X: %.1f Y: %.1f", camera->localPosition().x, camera->localPosition().y);

        ImGui::PopStyleColor();
    }
};

ADD_USER_COMPONENT(UserControls, upper)
