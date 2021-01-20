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

        ImVec2 c_size{350, 200};
        ImGui::SetCursorPos({(size.x - c_size.x) / 2.0f, (size.y - c_size.y) / 2.0f});
        if (ImGui::BeginChild("##settings", c_size, false,
                              ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration)) {
            ImGui::Text("Settings");

            if (ImGui::BeginTable("##s", 2)) {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);

                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Antialiasing");
                ImGui::TableNextColumn();
                //antyaliasing levels
                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Fullscreen");
                ImGui::TableNextColumn();
                //full levels
                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Resolution");
                ImGui::TableNextColumn();
                //full levels
                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Volume");
                ImGui::TableNextColumn();
                //volume levels

                ImGui::EndTable();
            }
        }

        ImGui::EndChild();
    }
};

ADD_USER_COMPONENT(SettingsGameWindow)
