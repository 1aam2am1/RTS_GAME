//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <Core/MonoBehaviour.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>

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

        }
        if (place_on_center("Settings")) {

        }
        if (place_on_center("Exit")) {
            Application::Quit();
        }
    }
};

ADD_USER_COMPONENT(MenuPlayGame)
