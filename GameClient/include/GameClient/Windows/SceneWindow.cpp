//
// Created by Michal_Marszalek on 19.11.2020.
//


#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/WindowLayout.h>

class SceneWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<SceneWindow>();
        WindowLayout::dockWindow(WindowLayout::Left, window);
        window->Show();
    }

    void OnGUI() override {

    }
};

MENU_ITEM(SceneWindow::Init, "Window/General/Scene", 3);
INITIALIZE_FUNC(SceneWindow::Init());
