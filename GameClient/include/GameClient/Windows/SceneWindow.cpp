//
// Created by Michal_Marszalek on 08.12.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include "Macro.h"
#include <imgui.h>
#include <GameClient/GlobalStaticVariables.h>
#include <imgui-SFML.h>
#include <GameClient/Windows/GameSceneMenuGizmo.h>

class SceneWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<SceneWindow>();
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->flags |= ImGuiWindowFlags_MenuBar;
        window->Show();
    }

    void OnGUI() override {
        ///TODO: Has it own camera implementation
#if UNITY_EDITOR
        DrawGameSceneGizmoMenu();
        ImGui::Image(global.m_target().getTexture());
        if (global.mis.draw_gizmo) {
            //draw box2d debug
        }
#endif
    }

};


MENU_ITEM(SceneWindow::Init, "Window/General/Scene", 5);

#if UNITY_EDITOR
INITIALIZE_FUNC(SceneWindow::Init());
#endif
