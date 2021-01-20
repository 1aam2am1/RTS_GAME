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
#include <Core/MonoBehaviour.h>
#include <Core/Application.h>
#include <Core/Attributes.h>

class GameWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<GameWindow>();
#if UNITY_EDITOR
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
#else
        window->flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window->flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
        window->flags |= ImGuiWindowFlags_NoBackground;
#endif
        window->Show();

        if (global.rendering.m_game_imGuiName.empty())
            global.rendering.m_game_imGuiName = window->imGuiName;
    }


    void OnDestroy() override {
        if (global.rendering.m_game_imGuiName == imGuiName) {
            global.rendering.m_game_imGuiName.clear();
        }
    }

    void OnStyleChange() override {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
#if !(UNITY_EDITOR)
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
#endif
    }

    void OnStylePop() override {
        ImGui::PopStyleVar();
#if !(UNITY_EDITOR)
        ImGui::PopStyleVar(2);
#endif
    }

    void Update() override {

        auto name = imGuiName;
        MainThread::Invoke([name]() {
            ImVec2 s{};
            if (ImGuiWindow *window = ImGui::FindWindowByName(name.c_str())) {
                s.x = window->ContentRegionRect.GetWidth();
                s.y = window->ContentRegionRect.GetHeight();
            }

            if (s.x > 0 && s.y > 0)
                global.rendering.m_target().create(std::ceil(s.x), std::ceil(s.y));
        });

    }

    void OnGUI() override {
#if UNITY_EDITOR
        DrawGameSceneGizmoMenu();
#endif

        ImGui::Image(global.rendering.m_target().getTexture());

        ImGui::SetCursorPos({0, 42});
        for (auto &object : global.scene.components) {
            auto mono = dynamic_pointer_cast<MonoBehaviour>(object);
            if (mono && mono->isActiveAndEnabled() &&
                (Application::isPlaying() ||
                 Attributes::CheckCustomAttribute(mono, ExecuteInEditMode))) {
                mono->OnGUI();
            }
        }
    }

protected:
};


MENU_ITEM(GameWindow::Init, "Window/General/Game", 6);

INITIALIZE_FUNC(GameWindow::Init());
