//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <Core/MonoBehaviour.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>

class MenuPlayGame : public MonoBehaviour {
public:
    void OnGUI() override {
        ImGuiStyle &style = ImGui::GetStyle();
        const auto window = ImGui::GetCurrentWindow();
        const auto size = window->ContentRegionRect.GetSize();

        auto place_on_center = [&](auto str) {
            auto is = ImGui::CalcTextSize(str).x + style.ItemSpacing.x;
            ImGui::SetCursorPos({(size.x - is) / 2, ImGui::GetCursorPos().y + 20.f});
            return ImGui::Button(str);
        };

        ImGui::SetCursorPos({0, (size.y - 120.0f) / 2.0f});

        if (place_on_center("Play")) {
            ImGui::OpenPopup("###play");
        }
        if (place_on_center("Settings")) {
            SceneManager::LoadScene("Assets/menu/Settings.unity");
        }
        if (place_on_center("Exit")) {
            Application::Quit();
        }

        if (ImGui::BeginPopup("###play", ImGuiWindowFlags_AlwaysAutoResize)) {
            auto width = ImGui::CalcTextSize("Single").x + ImGui::GetStyle().FramePadding.x * 2.0f;

            ImGui::SetNextItemWidth(width);
            if (ImGui::Button("Single")) {
                SceneManager::LoadScene("Assets/menu/Play.unity");
            }

            ImGui::SetNextItemWidth(width);
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            if (ImGui::Button("Multi")) {

            }
            ImGui::PopItemFlag();

            ImGui::SetNextItemWidth(width);
            if (ImGui::Button("Exit")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
};

ADD_USER_COMPONENT(MenuPlayGame)
