//
// Created by Michal_Marszalek on 17.11.2020.
//

#include "Editor.h"
#include "EditorUtility.h"
#include "DragAndDrop.h"
#include "EditorGuiLayout.h"
#include <GameClient/MetaData.h>
#include <GameApi/overload.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <execution>
#include <imgui_internal.h>

decltype(Editor::t_editor) Editor::t_editor;
decltype(Editor::t_fallback_editor) Editor::t_fallback_editor;

void Editor::OnSceneGUI() {

}

bool Editor::DrawDefaultInspector() {
    if (!target)
        return false;

    auto reflection = MetaData::getReflection(target.get());
    auto &types = reflection.getTPtrType;

    for (auto &it: reflection.getFields) {
        auto name = it.first;
        //TODO: Reformat names so that thy can be unique and so that m_ be deleted _ changed for ' ' and in Pascal Case
        if (name.size() > 2 && name[1] == '_') { name = &name[2]; }
        std::string key = "##";
        key += it.first.data();

        if (name == "enabled") { continue; }
        bool dirty = false;
        auto visitor = overload{
                [&name, &key, &dirty, &types](auto &&p) {
                    using type = function_traits_arg_t<decltype(p.first), 0>;

                    if (!p.second) { return; }

                    auto value = p.second();

                    if (!p.first) {
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                    }

                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if constexpr (std::is_same_v<type, int64_t>) {
                        dirty = ImGui::InputScalar(key.data(), ImGuiDataType_S64, &value, nullptr, nullptr);
                    } else if constexpr (std::is_same_v<type, float>) {
                        dirty = ImGui::InputScalar(key.data(), ImGuiDataType_Float, &value, nullptr, nullptr);
                    } else if constexpr (std::is_same_v<type, double>) {
                        dirty = ImGui::InputScalar(key.data(), ImGuiDataType_Double, &value, nullptr, nullptr);
                    } else if constexpr (std::is_same_v<type, std::string>) {
                        dirty = ImGui::InputText(key.data(), &value);
                    } else if constexpr (std::is_same_v<type, bool>) {
                        dirty = ImGui::Checkbox(key.data(), &value);
                    } else if constexpr (std::is_same_v<type, sf::Color>) {
                        float color[4];
                        color[0] = value.r / 255.f;
                        color[1] = value.g / 255.f;
                        color[2] = value.b / 255.f;
                        color[3] = value.a / 255.f;

                        dirty = ImGui::ColorEdit4(key.data(), color,
                                                  ImGuiColorEditFlags_NoInputs |
                                                  ImGuiColorEditFlags_NoTooltip |
                                                  ImGuiColorEditFlags_AlphaPreviewHalf);
                        if (dirty) {
                            value.r = color[0] * 255.f;
                            value.g = color[1] * 255.f;
                            value.b = color[2] * 255.f;
                            value.a = color[3] * 255.f;
                        }

                    } else if constexpr (std::is_same_v<type, TPtr<Object>>) {
                        std::type_index type = typeid(Object);
                        auto it = types.find(name);
                        if (it != types.end()) {
                            type = it->second;
                        }

                        auto v2 = EditorGUILayout::ObjectField(value, type, true);
                        if (v2 != value) {
                            dirty = true;
                            value = v2;
                        }
                    } else if constexpr (is_instance_v<type, sf::Vector2>) {
                        auto v2 = EditorGUILayout::Vector2Field(value);

                        if (v2 != value) {
                            dirty = true;
                            value = v2;
                        }
                    } else if constexpr (is_instance_v<type, sf::Vector3>) {
                        auto v2 = EditorGUILayout::Vector3Field(value);

                        if (v2 != value) {
                            dirty = true;
                            value = v2;
                        }
                    } else if constexpr (is_instance_v<type, sf::Rect>) {
                        auto v2 = EditorGUILayout::RectField(value);

                        if (v2 != value) {
                            dirty = true;
                            value = v2;
                        }
                    } else if constexpr (std::is_same_v<type, nlohmann::json>) {
                        std::type_index type = typeid(Object);
                        auto it = types.find(name);
                        if (it != types.end()) {
                            type = it->second;
                        }

                        if (!Enums::getReflection(type).d.members.empty()) {
                            auto v2 = EditorGUILayout::EnumField(value.template get<std::string>(), type);

                            if (v2 != value) {
                                dirty = true;
                                value = v2;
                            }
                        } else {
                            ImGuiContext &g = *GImGui;
                            g.NextItemData.Flags &= ~ImGuiNextItemDataFlags_HasWidth;
                            ImGui::NewLine();
                        }
                    } else {
                        ImGuiContext &g = *GImGui;
                        g.NextItemData.Flags &= ~ImGuiNextItemDataFlags_HasWidth;
                        ImGui::NewLine();
                    }
                    //TODO: !!! Next Editor default widgets

                    if (!p.first) {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleVar();
                    }
                    if (dirty && p.first) {
                        p.first(value);
                    }
                }
        };

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", name.data());
        ImGui::SameLine();
        std::visit(visitor, it.second);

        if (dirty) {
            EditorUtility::SetDirty(target);
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
        name += " -> no editor##" + GameApi::to_string(editor->target.get());

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
        previousEditor = editor;

        previous.emplace_back(targetObject, editor, editorType);
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
        c->Awake();
        return c;
    }

    it = std::find_if(std::execution::par_unseq, t_editor.begin(), t_editor.end(),
                      [&](auto &&i) { return i.second.editorForChildClasses && i.second.check_child(targetObject); });

    if (it != t_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        c->Awake();
        return c;
    }
///////////////////////////////////////////////////////////////////////////////////////
    it = std::find_if(std::execution::par_unseq, t_fallback_editor.begin(), t_fallback_editor.end(),
                      [&](auto &&i) { return i.first == editorType; });

    if (it != t_fallback_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        c->Awake();
        return c;
    }

    it = std::find_if(std::execution::par_unseq, t_fallback_editor.begin(), t_fallback_editor.end(),
                      [&](auto &&i) { return i.second.editorForChildClasses && i.second.check_child(targetObject); });

    if (it != t_fallback_editor.end()) {
        auto c = it->second.create();
        c->target = targetObject;
        c->Awake();
        return c;
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    if (targetObject && editorType != typeid(nullptr)) {
        GameApi::log(ERR.fmt("Didn't find editor for given type %s trying default editor",
                             GameApi::demangle(editorType.name()).data()));

        return CreateEditor(TPtr<Object>{});
    }
    GameApi::log(ERR.fmt("Didn't find default editor"));

    return {};
}
