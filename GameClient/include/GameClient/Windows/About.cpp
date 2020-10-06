//
// Created by Michal_Marszalek on 27.09.2020.
//

#include <Macro.h>
#include <Editor/Menu.h>
#include <imgui.h>
#include <Version/version.h>

class About : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<About>();
        window->ShowUtility();
    }

    void Awake() override {
        titleContent = "About";
    }

    void OnGUI() override {
        ImGui::Text("RTS_GAME");
        ImGui::Separator();
        ImGui::Text("Engineering Thesis");
        ImGui::Separator();
        ImGui::Text("Author: Michal Marszalek");
        ImGui::Separator();
        ImGui::TextWrapped("Version: %s %i bit\n", (Version::GIT_TAG + " " +
                                                    Version::GIT_SHA + " " +
                                                    Version::DATE).c_str(),
                           sizeof(void *) * 8);
    }
};

MENU_ITEM(About::Init, "Help/About Game", -9)
