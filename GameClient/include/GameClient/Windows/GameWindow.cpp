//
// Created by Michal_Marszalek on 08.12.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include "Macro.h"
#include <imgui.h>

class GameWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<GameWindow>();
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->Show();
    }

    void OnGUI() override {

    }

protected:
#if !(UNITY_EDITOR)
    void drawGui() override {
        static_assert(false, "Run in full window mode");
    }
#endif
};


MENU_ITEM(GameWindow::Init, "Window/General/Game", 6);

INITIALIZE_FUNC(GameWindow::Init());
