//
// Created by Michal_Marszalek on 08.12.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include "Macro.h"
#include <imgui.h>
#include <GameClient/GlobalStaticVariables.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>
#include <GameClient/MainThread.h>
#include <GameClient/Windows/GameSceneMenuGizmo.h>

class GameWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<GameWindow>();
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->flags |= ImGuiWindowFlags_MenuBar;
        window->Show();
    }

    void Update() override {
#if UNITY_EDITOR
        auto name = imGuiName;
        MainThread::Invoke([name]() {
            ImVec2 s{};
            if (ImGuiWindow *window = ImGui::FindWindowByName(name.c_str())) {
                s.x = window->ContentRegionRect.GetWidth();
                s.y = window->ContentRegionRect.GetHeight();
            }

            if (s.x > 0 && s.y > 0)
                global.m_target().create(s.x, s.y);
        });
#endif
    }

    void OnGUI() override {
#if UNITY_EDITOR
        DrawGameSceneGizmoMenu();
        ImGui::Image(global.m_target().getTexture());
#else
        //Target is RenderTexture therefore it is drown on main screen
#endif
    }

protected:
#if !(UNITY_EDITOR)
    void drawGui() override {
        //static_assert(false, "Run in full window mode");
    }
#endif
};


MENU_ITEM(GameWindow::Init, "Window/General/Game", 6);

INITIALIZE_FUNC(GameWindow::Init());
