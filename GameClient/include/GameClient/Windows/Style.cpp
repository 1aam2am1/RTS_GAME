//
// Created by Michal_Marszalek on 01.10.2020.
//

#include <Macro.h>
#include <Editor/Menu.h>
#include <imgui.h>

struct Style : public EditorWindow {

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Style>();
        window->ShowUtility();
    }

protected:
    void OnGUI() override {
        ImGui::ShowStyleEditor();
    }
};

MENU_ITEM(Style::Init, "Help/Style", 2);
