//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "EditorGuiLayout.h"
#include <GameClient/Unity/Core/Object.h>
#include <GameClient/Unity/Editor/DragAndDrop.h>
#include <GameClient/MainThread.h>
#include <map>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>
#include <Macro.h>

static bool cleared = true;
static std::map<ImGuiID, uint16_t> window_ob;

std::string GetKey() {
    if (cleared) {
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
            ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
            ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        if (DragAndDrop::IsDragging()/* && DragAndDrop::GetGenericData("OBJECT")*/) {
            DragAndDrop::visualMode = DragAndDrop::Link;
            //TODO: Make it better, type and so on
        }
        if (ImGui::IsMouseReleased(0) && DragAndDrop::IsDragging()) {
            DragAndDrop::AcceptDrag();
            auto new_value = DragAndDrop::GetGenericData("OBJECT");

            auto reflection = MetaData::getReflection(objType);
            if (new_value && reflection.CheckInstance(new_value.get())) {
                return new_value;
            }

            for (auto &it: DragAndDrop::objectReferences) {
                if (reflection.CheckInstance(it.get())) {
                    return it;
                }
            }

            return value;
        }
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
        ImGui::IsMouseDoubleClicked(1)) {

        return {};
    }

    return value;
}

sf::FloatRect EditorGUILayout::RectField(sf::FloatRect rect) {
    std::string key = GetKey();

    if (ImGui::BeginTable(key.data(), 4)) {
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
        ImGui::InputFloat("##x", &rect.left);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##y", &rect.top);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("W");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##w", &rect.width);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("H");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##h", &rect.height);

        ImGui::EndTable();
    }

    return rect;
}

sf::IntRect EditorGUILayout::RectField(sf::IntRect rect) {
    std::string key = GetKey();

    if (ImGui::BeginTable(key.data(), 4)) {
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
        ImGui::InputInt("##x", &rect.left);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##y", &rect.top);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("W");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##w", &rect.width);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("H");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##h", &rect.height);

        ImGui::EndTable();
    }

    return rect;
}

sf::Vector2i EditorGUILayout::Vector2Field(sf::Vector2i vec) {
    std::string key = GetKey();

    if (ImGui::BeginTable(key.data(), 4)) {
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
        ImGui::InputInt("##x", &vec.x);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##y", &vec.y);

        ImGui::EndTable();
    }

    return vec;
}

sf::Vector2f EditorGUILayout::Vector2Field(sf::Vector2f vec) {
    std::string key = GetKey();

    if (ImGui::BeginTable(key.data(), 4)) {
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
        ImGui::InputFloat("##x", &vec.x);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##y", &vec.y);

        ImGui::EndTable();
    }

    return vec;
}

sf::Vector3i EditorGUILayout::Vector3Field(sf::Vector3i vec) {
    std::string key = GetKey();

    ImGui::NewLine();
    if (ImGui::BeginTable(key.data(), 6)) {
        auto width = ImGui::CalcTextSize("X").x;

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);

        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("X");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##x", &vec.x);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##y", &vec.y);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("W");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputInt("##w", &vec.z);

        ImGui::EndTable();
    }

    return vec;
}

sf::Vector3f EditorGUILayout::Vector3Field(sf::Vector3f vec) {
    std::string key = GetKey();

    ImGui::NewLine();
    if (ImGui::BeginTable(key.data(), 6)) {
        auto width = ImGui::CalcTextSize("X").x;

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, width);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);

        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("X");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##x", &vec.x);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Y");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##y", &vec.y);
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("W");
        ImGui::TableNextColumn();

        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputFloat("##w", &vec.z);

        ImGui::EndTable();
    }

    return vec;
}

std::string EditorGUILayout::EnumField(std::string str, std::type_index type) {
    auto reflection = Enums::getReflection(type);
    std::string key = GetKey();

    if (reflection.d.members.empty()) {
        ImGui::TextDisabled("%s", str.data());
        return str;
    } else {
        if (ImGui::BeginCombo(key.data(), str.data())) {
            for (auto &&s : reflection.d.members) {
                const bool is_selected = (s.second == str);
                if (ImGui::Selectable(s.second.data(), is_selected))
                    str = s.second;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        return str;
    }
}
