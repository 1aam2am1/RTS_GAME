//
// Created by Michal_Marszalek on 09.11.2020.
//

#include <Macro.h>
#include <Editor/Menu.h>
#include <imgui.h>

struct DemoWindow : public EditorWindow {

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<DemoWindow>();
        window->ShowUtility();
    }

protected:
    void drawGui() override {
        if (work != NotShown) {
            bool open = true;
            ImGui::ShowDemoWindow(&open);

            if (!open) { Close(); }
        }
    }
};

MENU_ITEM(DemoWindow::Init, "Help/Demo", 1);
