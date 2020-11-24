//
// Created by Michal_Marszalek on 17.11.2020.
//

#include "Editor.h"
#include "EditorUtility.h"
#include <GameClient/MetaData.h>
#include <GameApi/overload.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <execution>

decltype(Editor::t_editor) Editor::t_editor;
decltype(Editor::t_fallback_editor) Editor::t_fallback_editor;

void Editor::OnSceneGUI() {

}

bool Editor::DrawDefaultInspector() {
    if (!target)
        return false;

    auto reflection = MetaData::get_reflections(target.get());
/* int64_t *,
            double *,
            std::string *,
            bool *,
            TPtr<Object> *,
            nlohmann::json *,
            std::function<void(int64_t)>,
            std::function<void(double)>,
            std::function<void(std::string)>
*/

    for (auto &it: reflection) {
        for (auto &it2 : it->get(target.get())) {
            auto &name = it2.first;
            //TODO: Reformat names so that thy can be unique and so that m_ be deleted _ changed for ' ' and in Pascal Case
            if (name.size() > 2 && name[1] == '_') { name = &name[2]; }
            std::string key = "##";
            key += it2.first.data();

            bool dirty = false;
            auto visitor = overload{
                    [&key, &dirty](int64_t *i) {
                        ImGui::SetNextItemWidth(-1.0f);
                        dirty = ImGui::InputScalar(key.data(), ImGuiDataType_S64, i, nullptr, nullptr);
                    },
                    [&key, &dirty](double *d) {
                        ImGui::SetNextItemWidth(-1.0f);
                        dirty = ImGui::InputDouble(key.data(), d);
                    },
                    [&key, &dirty](std::string *str) {
                        ImGui::SetNextItemWidth(-1.0f);
                        dirty = ImGui::InputText(key.data(), str);
                    },
                    [&key, &dirty](bool *b) {
                        ImGui::SetNextItemWidth(-1.0f);
                        dirty = ImGui::Checkbox(key.data(), b);
                    },
                    [&key, &dirty](TPtr<Object> *o) {
                        //TODO: Drop target
                    },
                    [&key, &dirty](nlohmann::json *) {},
                    [&key, &dirty](std::function<void(int64_t)>) {
                        //TODO: Find from cget
                    },
                    [&key, &dirty](std::function<void(double)>) {
                        //TODO: Find from cget
                    },
                    [&key, &dirty](std::function<void(std::string)>) {
                        //TODO: Find from cget
                    }
            };

            ImGui::Text("%s", name.data());
            ImGui::SameLine();
            std::visit(visitor, it2.second);

            if (dirty) {
                EditorUtility::SetDirty(target);
            }
        }
    }

    return true;
}

std::string Editor::GetPreviewTitle() {
    if (target)
        return GameApi::demangle(typeid(*target.get()).name());

    return {};
}

void Editor::DrawFoldoutInspector(TPtr<Object> target, TPtr<Editor> &editor) {
    if (!editor) {
        editor = CreateEditor(target);
    }

    if (editor) {
        std::string name = editor->GetPreviewTitle();
        if (name.empty()) { name = editor->Editor::GetPreviewTitle(); }
        name += "###" + GameApi::to_string(editor->target.get());

        bool open = ImGui::TreeNode(name.data());
        if (open) {
            editor->OnInspectorGUI();
            editor->OnSceneGUI();
            ImGui::TreePop();
        };
    } else if (target) {
        std::string name = GameApi::demangle(typeid(*target.get()).name());
        name += " -> no editor###" + GameApi::to_string(editor->target.get());

        bool open = ImGui::TreeNode(name.data());
        if (open) {
            ImGui::TreePop();
        };
    }
}

void Editor::CreateCachedEditor(TPtr<Object> targetObject, TPtr<Editor> &previousEditor, std::type_index editorType) {
    static std::list<std::tuple<TPtr<Object>, TPtr<Editor>, std::type_index>> previous;

    if (editorType == typeid(nullptr)) {
        editorType = targetObject ? typeid(*targetObject.get()) : typeid(nullptr);
    }

    // Remove deleted objects from editor -> clear list
    previous.remove_if([](auto &&i) {
        return std::get<0>(i).expired() && std::get<2>(i) != typeid(nullptr);
    });

    auto it = std::find_if(std::execution::par_unseq, previous.begin(), previous.end(), [&targetObject](auto &&i) {
        return std::get<0>(i) == targetObject;
    });

    if (it != previous.end()) {
        if (std::get<1>(*it).expired() || std::get<2>(*it) != editorType) {
            std::get<1>(*it) = CreateEditor(targetObject, editorType);
            std::get<2>(*it) = editorType;
        }
        previousEditor = std::get<1>(*it);
    } else {
        auto editor = CreateEditor(targetObject, editorType);
        TPtr<Object> t{nullptr};

        t = targetObject;
        previousEditor = editor;

        previous.emplace_back(t, editor, editorType);
    }
}

TPtr<Editor> Editor::CreateEditor(TPtr<Object> targetObject, std::type_index editorType) {
    if (editorType == typeid(nullptr)) {
        editorType = targetObject ? typeid(*targetObject.get()) : typeid(nullptr);
    }

    auto it = std::find_if(std::execution::par_unseq, t_editor.begin(), t_editor.end(),
                           [&](auto &&i) { return i.first == editorType; });

    if (it != t_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        return c;
    }

    it = std::find_if(std::execution::par_unseq, t_editor.begin(), t_editor.end(),
                      [&](auto &&i) { return i.second.editorForChildClasses && i.second.check_child(targetObject); });

    if (it != t_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        return c;
    }
///////////////////////////////////////////////////////////////////////////////////////
    it = std::find_if(std::execution::par_unseq, t_fallback_editor.begin(), t_fallback_editor.end(),
                      [&](auto &&i) { return i.first == editorType; });

    if (it != t_fallback_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        return c;
    }

    it = std::find_if(std::execution::par_unseq, t_fallback_editor.begin(), t_fallback_editor.end(),
                      [&](auto &&i) { return i.second.editorForChildClasses && i.second.check_child(targetObject); });

    if (it != t_fallback_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        return c;
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    if (targetObject && editorType != typeid(nullptr)) {
        GameApi::log(ERR.fmt("Didn't find editor for given type %s trying default editor",
                             GameApi::demangle(editorType.name()).data()));

        return CreateEditor(TPtr<Object>{nullptr});
    }
    GameApi::log(ERR.fmt("Didn't find default editor"));

    return TPtr<Editor>(nullptr);
}
