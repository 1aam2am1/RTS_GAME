//
// Created by Michal_Marszalek on 04.12.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Component.h>
#include <Editor/EditorUtility.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <Core/Behaviour.h>


class GameObjectEditor : public Editor {
public:

    void OnInspectorGUI() override {
        auto d = dynamic_pointer_cast<GameObject>(target);
        if (!d) { return; }
        bool dirty = false;

        {
            bool b = d->activeSelf();
            dirty |= ImGui::Checkbox("##active", &b);
            if (dirty) { d->SetActive(b); }
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(-FLT_MIN);
        dirty |= ImGui::InputText("##name", &d->name);

        //tag
        auto width = ImGui::GetWindowContentRegionWidth();
        width = width - ImGui::CalcTextSize("Tag").x - ImGui::CalcTextSize("Layer").x -
                3.f * ImGui::GetStyle().ItemSpacing.x;

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Tag");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(width / 2.f);
        dirty |= ImGui::InputText("##tag", &d->tag);
        ImGui::SameLine();

        //layer
        ImGui::Text("Layer");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(width / 2.f);
        dirty |= ImGui::InputScalar("##layer", ImGuiDataType_S64, &d->layer, nullptr, nullptr);

        if (dirty) { EditorUtility::SetDirty(d); }

        auto components = d->GetComponents<Component>();
        for (auto &c : components) {
            std::type_index type = typeid(*c.get());
            auto reflection = MetaData::getReflection(type);

            bool open = ImGui::CollapsingHeader(("##" + GameApi::to_string(c.get())).data(),
                                                ImGuiTreeNodeFlags_DefaultOpen); //ImGuiTreeNodeFlags_OpenOnArrow
            ImGui::SameLine();

            auto behaviour = dynamic_pointer_cast<Behaviour>(c);
            if (behaviour) {
                ImGui::Checkbox("##behaviour", &behaviour->enabled);
            } else {
                auto total_bb = ImVec2(ImGui::GetFrameHeight(),
                                       ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f);

                ImGui::InvisibleButton("##behaviour", total_bb);
            }

            ImGui::SameLine();
            ImGui::Text("%s", reflection.name.data());

            if (open) {
                TPtr<Editor> editor;
                Editor::CreateCachedEditor(c, editor, type);

                editor->OnInspectorGUI();
            }
        }
    }
};

CUSTOM_EDITOR_FALLBACK(GameObjectEditor, GameObject);
