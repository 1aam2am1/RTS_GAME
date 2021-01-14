//
// Created by Michal_Marszalek on 14.01.2021.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/Camera.h>
#include <GameClient/Unity/Macro.h>
#include <imgui.h>

class CameraEditor : public Editor {
public:
    TPtr<Camera> d;

    void Awake() override {
        d = dynamic_pointer_cast<Camera>(target);
    }

    void OnInspectorGUI() override {
        if (!d) { return; }

        DrawDefaultInspector();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Viewport Rect");
        ImGui::SameLine();

        if (ImGui::BeginTable("##rect", 4)) {
            auto width = ImGui::CalcTextSize("X").x;

            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);

            ImGui::TableNextColumn();

            ImGui::AlignTextToFramePadding();
            ImGui::Text("X");
            ImGui::TableNextColumn();

            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("##x", &d->pixelRect.left);
            ImGui::TableNextColumn();

            ImGui::AlignTextToFramePadding();
            ImGui::Text("Y");
            ImGui::TableNextColumn();

            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("##y", &d->pixelRect.top);
            ImGui::TableNextColumn();

            ImGui::AlignTextToFramePadding();
            ImGui::Text("W");
            ImGui::TableNextColumn();

            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("##w", &d->pixelRect.width);
            ImGui::TableNextColumn();

            ImGui::AlignTextToFramePadding();
            ImGui::Text("H");
            ImGui::TableNextColumn();

            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputFloat("##h", &d->pixelRect.height);

            ImGui::EndTable();
        }
    }
};

CUSTOM_EDITOR_FALLBACK(CameraEditor, Camera);

