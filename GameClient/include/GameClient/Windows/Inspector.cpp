//
// Created by Michal_Marszalek on 24.09.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"

class Inspector : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Inspector>();
        window->Show();
    }

    void Update() override {

    }

    void OnGUI() override {

    }
};

MENU_ITEM(Inspector::Init, "Window/General/Inspector", 4);
