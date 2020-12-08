//
// Created by Michal_Marszalek on 08.12.2020.
//

#include "imgui_custom.h"
#include <imgui_internal.h>

bool ImGui::SelectableInput(const char *str_id, bool selected, std::string *str, ImGuiSelectableFlags flags) {
    IM_ASSERT(str != nullptr);

    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    ImVec2 pos_before = window->DC.CursorPos;

    PushID(str_id);
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.FramePadding.y * 2.0f));
    bool ret = Selectable("##Selectable", selected,
                          flags | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap);
    PopStyleVar();

    ImGuiID id = window->GetID("##Input");
    bool temp_input_is_active = TempInputIsActive(id);
    bool temp_input_start = ret ? IsMouseDoubleClicked(0) : false;

    if (temp_input_start)
        SetActiveID(id, window);

    if (temp_input_is_active || temp_input_start) {
        ImVec2 pos_after = window->DC.CursorPos;
        window->DC.CursorPos = pos_before;
        {
            if (str->capacity() <= (str->size() + 4))
                str->reserve(str->size() + 8);

            ret = TempInputText(window->DC.LastItemRect, id, "##Input", str->data(), str->capacity() + 1,
                                ImGuiInputTextFlags_None);
        }
        window->DC.CursorPos = pos_after;
    } else {
        window->DrawList->AddText(pos_before, GetColorU32(ImGuiCol_Text), str->c_str());
    }

    PopID();
    return ret;
}
