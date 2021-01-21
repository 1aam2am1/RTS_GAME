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
#include <Editor/Menu.h>


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
            if (!d) { return; }
            ImGui::PushID(c.get());
            bool destroy = false;

            auto reflection = MetaData::getReflection(c.get());

            bool open = ImGui::CollapsingHeader(("##" + GameApi::to_string(c.get())).data(),
                                                ImGuiTreeNodeFlags_DefaultOpen |
                                                ImGuiTreeNodeFlags_AllowItemOverlap); //ImGuiTreeNodeFlags_OpenOnArrow

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Remove")) {
                    destroy = true;
                }

                ImGui::EndPopup();
            }

            ImGui::SameLine();

            auto it = std::find_if(reflection.getFields.begin(), reflection.getFields.end(),
                                   [](auto &&it) { return it.first == "m_enabled"; });
            if (it != reflection.getFields.end() && it->second.index() == 4) {
                auto set_get = std::get<4>(it->second);
                bool enabled = set_get.second();
                dirty = ImGui::Checkbox("##behaviour", &enabled);

                if (dirty) {
                    set_get.first(enabled);
                    EditorUtility::SetDirty(c);
                }
            } else {
                auto total_bb = ImVec2(ImGui::GetFrameHeight(),
                                       ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f);

                ImGui::InvisibleButton("##behaviour", total_bb);
            }

            ImGui::SameLine();
            ImGui::Text("%s", reflection.name.data());

            if (open) {
                TPtr<Editor> editor;
                Editor::CreateCachedEditor(c, editor, reflection.type);

                if (editor)
                    editor->OnInspectorGUI();
            }

            if (destroy) {
                Object::DestroyImmediate(c);
            }

            ImGui::PopID();
        }

        ImGui::Separator();
        {
            ImVec2 LocalButtonSize = {100, 40};
            auto pos = ImGui::GetWindowWidth() - LocalButtonSize.x;
            pos /= 2.0f;

            if (pos < ImGui::GetCursorPosX()) {
                pos = ImGui::GetCursorPosX();
            }

            ImGui::SetCursorPosX(pos);

            auto clicked = ImGui::Button("Components", LocalButtonSize);
            if (clicked) {
                ContextMenu::Init("Component");
            }

        }
    }
};

CUSTOM_EDITOR_FALLBACK(GameObjectEditor, GameObject);
