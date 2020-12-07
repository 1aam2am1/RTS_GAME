//
// Created by Michal_Marszalek on 07.12.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/Macro.h>
#include <Editor/EditorUtility.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

class TransformEditor : public Editor {
public:
    void OnInspectorGUI() override {
        auto d = dynamic_pointer_cast<Transform>(target);
        if (!d) { return; }

        auto position = d->localPosition();
        auto z_rotation = d->localRotation();
        auto scale = d->localScale();

        bool dirty = false;

        if (ImGui::BeginTable("##table2", 4)) {
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed, 55.f);
            ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed, 55.f);
            ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_WidthFixed, 55.f);
            //ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = ImGui::GetStyle().FramePadding.y;
            ///row1
            ImGui::Text("Position");
            ImGui::TableNextColumn();

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##x", &position.x, 0.05f, 0.f, 0.f, "%.2f");
            ImGui::TableNextColumn();

            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##y", &position.y, 0.05f, 0.f, 0.f, "%.2f");
            ImGui::TableNextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##z", &position.z, 0.05f, 0.f, 0.f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = ImGui::GetStyle().FramePadding.y;
            ///row2
            ImGui::Text("Rotation");
            ImGui::TableNextColumn();
            ImGui::TableNextColumn();
            ImGui::TableNextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##r_z", &z_rotation, 0.05f, 0.f, 0.f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = ImGui::GetStyle().FramePadding.y;
            //row3
            ImGui::Text("Scale");
            ImGui::TableNextColumn();

            ImGui::Text("X");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##s_x", &scale.x, 0.05f, 0.f, 0.f, "%.2f");
            ImGui::TableNextColumn();

            ImGui::Text("Y");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##s_y", &scale.y, 0.05f, 0.f, 0.f, "%.2f");
            ImGui::TableNextColumn();

            ImGui::Text("Z");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            dirty |= ImGui::DragFloat("##s_z", &scale.z, 0.05f, 0.f, 0.f, "%.2f");


            ImGui::EndTable();
        }

        if (dirty) {
            d->localPosition = position;
            d->localRotation = z_rotation;
            d->localScale = scale;

            EditorUtility::SetDirty(d);
        }
    }
};

CUSTOM_EDITOR_FALLBACK(TransformEditor, Transform);
