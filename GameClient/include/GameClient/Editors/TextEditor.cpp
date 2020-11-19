//
// Created by Michal_Marszalek on 19.11.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/TextAsset.h>
#include <GameClient/Unity/Macro.h>
#include <imgui.h>

class TextEditor : public Editor {
public:
    void OnInspectorGUI() override {
        auto d = dynamic_pointer_cast<TextAsset>(target);
        if (!d) { return; }

        ImGui::Text("Text: ");
        ImGui::SameLine();
        ImGui::TextWrapped("%s", d->text.data());

        DrawDefaultInspector();
    }
};

CUSTOM_EDITOR_FALLBACK(TextEditor, TextAsset);
