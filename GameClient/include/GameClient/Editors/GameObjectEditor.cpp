//
// Created by Michal_Marszalek on 04.12.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Component.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

DEFINE_BREAK_IN_CLASS()

DEFINE_BREAK_IN(, m_active, &GameObject::m_active)

class GameObjectEditor : public Editor {
public:

    void OnInspectorGUI() override {
        auto d = dynamic_pointer_cast<GameObject>(target);
        if (!d) { return; }

        ImGui::Checkbox("##active", &((*d.get()).*break_in(m_active())));
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);
        ImGui::InputText("##name", &d->name);

        //tag
        auto width = ImGui::GetWindowContentRegionWidth();
        width = width - ImGui::CalcTextSize("Tag").x - ImGui::CalcTextSize("Layer").x -
                3.f * ImGui::GetStyle().ItemSpacing.x;
        ImGui::Text("Tag");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(width / 2.f);
        ImGui::InputText("##tag", &d->tag);
        ImGui::SameLine();

        //layer
        ImGui::Text("Layer");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(width / 2.f);
        ImGui::InputScalar("##layer", ImGuiDataType_S64, &d->layer, nullptr, nullptr);

        auto components = d->GetComponents<Component>();
        for (auto &c : components) {
            std::type_index type = typeid(*c.get());
            auto reflection = MetaData::getReflection(type);

            std::string key{reflection.name};
            key += "##" + GameApi::to_string(c.get());
            if (ImGui::CollapsingHeader(key.data())) {
                TPtr<Editor> editor;
                Editor::CreateCachedEditor(c, editor, type);

                editor->OnInspectorGUI();
            }
        }
    }
};

CUSTOM_EDITOR_FALLBACK(GameObjectEditor, GameObject);
