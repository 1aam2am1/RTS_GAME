//
// Created by Michal_Marszalek on 08.12.2020.
//

#ifndef RTS_GAME_IMGUICUSTOM_H
#define RTS_GAME_IMGUICUSTOM_H

#include <string>
#include <imgui.h>

namespace ImGui {
    bool SelectableInput(const char *str_id, bool selected, std::string *str,
                         ImGuiSelectableFlags = ImGuiSelectableFlags_None);
}


#endif //RTS_GAME_IMGUICUSTOM_H
