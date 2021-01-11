//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "EditorGuiLayout.h"
#include <GameClient/Unity/Core/Object.h>
#include <GameClient/Unity/Editor/DragAndDrop.h>
#include <GameApi/Compiler.h>
#include <GameClient/MainThread.h>
#include <map>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <GameClient/Unity/Core/Sprite.h>

static bool cleared = true;
static std::map<ImGuiID, uint16_t> window_ob;

std::string GetKey() {
    if (!cleared) {
        cleared = false;
        MainThread::Invoke([]() {
            cleared = true;
            std::for_each(window_ob.begin(), window_ob.end(), [](auto &it) {
                it.second = 0;
            });
        });
    }

    return "##GetKey_" + GameApi::to_string(window_ob[ImGui::GetCurrentWindow()->ID]++);
}

TPtr<> EditorGUILayout::ObjectField(TPtr<> value, std::type_index objType, bool allowSceneObjects) {
    std::string name;
    bool dirty = false;
    std::string key = GetKey();

    if (value) {
        name = GameApi::demangle(typeid(*value.get()).name()) + ": " + value->name;
    } else {
        name = GameApi::demangle(objType.name()) + ": None";
    }

    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::InputText(key.data(), &name);
    ImGui::PopItemFlag();
    if (ImGui::IsItemHovered(
            ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        if (DragAndDrop::IsDragging()/* && DragAndDrop::GetGenericData("OBJECT")*/) {
            DragAndDrop::visualMode = DragAndDrop::Link;
            //TODO: Make it better, type and so on
        }
        if (ImGui::IsMouseReleased(0) && DragAndDrop::IsDragging()) {
            DragAndDrop::AcceptDrag();
            value = DragAndDrop::GetGenericData("OBJECT");
            dirty = true;
        }
    }
    if (dirty) {
        /*if (objType == typeid(Sprite) && typeid(*value.get()) == typeid(Texture2D)) {
            auto t = static_pointer_cast<Texture2D>(value);

            return Sprite::Create(t, sf::FloatRect{0, 0,
                                                   static_cast<float>(t->t0.getSize().x),
                                                   static_cast<float>(t->t0.getSize().y)});
        }*/

        if (value && objType == typeid(*value)) {
            return value;
        }

        for (auto &it: DragAndDrop::objectReferences) {
            if (objType == typeid(*it)) {
                return it;
            }
        }
    }
    return TPtr<>();
}
