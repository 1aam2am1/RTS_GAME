//
// Created by Michal_Marszalek on 13.01.2021.
//

#include <GameClient/Unity/Editor/AssetImporterEditor.h>
#include <GameClient/Importers/TextureImporter.h>
#include <GameClient/Unity/Macro.h>
#include <Editor/EditorUtility.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

class TextureImporterEditor : public AssetImporterEditor {
public:
    TPtr<TextureImporter> d;

    void Awake() override {
        d = dynamic_pointer_cast<TextureImporter>(target);
    }

    void OnInspectorGUI() override {
        if (!d) { return; }

        ImGui::Text("pixelsPerUnit");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputScalar("#pixelsPerUnit", ImGuiDataType_Float, &d->pixelsPerUnit, nullptr, nullptr);

        ImGui::Text("Force settings");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::Checkbox("##pixelsPerUnit", &d->force_sprites);

        if (ImGui::Button("Apply")) {
            EditorUtility::SetDirty(target);
            d->SaveAndReimport();
        }
    }
};


CUSTOM_EDITOR(TextureImporterEditor, TextureImporter);
