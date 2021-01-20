//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <imgui.h>
#include <Core/MonoBehaviour.h>
#include <Macro.h>
#include <imgui_internal.h>
#include <GameClient/Unity/Editor/Menu.h>

class SettingsGameWindow : public MonoBehaviour {
public:
    void OnGUI() override {
        ImGuiStyle &style = ImGui::GetStyle();
        const auto window = ImGui::GetCurrentWindow();
        const auto size = window->ContentRegionRect.GetSize();
/*
        auto place_on_center = [&](auto str) {
            auto is = ImGui::CalcTextSize(str).x + style.ItemSpacing.x;
            ImGui::SetCursorPos({(size.x - is) / 2, ImGui::GetCursorPos().y + 20.f});
            return ImGui::Button(str);
        };*/

        ImGui::SetCursorPos({0, (size.y - 120.0f) / 2.0f});
    }
};

ADD_USER_COMPONENT(SettingsGameWindow)
