//
// Created by Michal_Marszalek on 01.10.2020.
//

#include <Macro.h>
#include <Editor/Menu.h>
#include <imgui.h>

struct Metrics : public EditorWindow {

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Metrics>();
        window->ShowUtility();
    }

protected:
    void drawGui() override {
        if (work != NotShown) {
            bool open = true;
            ImGui::ShowMetricsWindow(&open);

            if (!open) { Close(); }
        }
    }
};

MENU_ITEM(Metrics::Init, "Help/Metrics", 1);
