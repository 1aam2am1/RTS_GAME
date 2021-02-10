//
// Created by Michal_Marszalek on 2021-02-08.
//

#include "TextCenter.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(TextCenter)

void TextCenter::OnGUI() {
    ImGuiStyle &style = ImGui::GetStyle();
    const auto window = ImGui::GetCurrentWindow();
    const auto size = window->ContentRegionRect.GetSize();

    ImGui::SetWindowFontScale(10);

    auto str = name.data();
    auto is = ImGui::CalcTextSize(str).x + style.ItemSpacing.x;
    ImGui::SetCursorPos({(size.x - is) / 2, ImGui::GetCursorPos().y + 20.f});
    ImGui::Text("%s", str);

    ImGui::SetWindowFontScale(1);
}
