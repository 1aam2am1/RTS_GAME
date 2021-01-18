//
// Created by Michal_Marszalek on 2021-01-18.
//

#ifndef RTS_GAME_SCENEWINDOW_H
#define RTS_GAME_SCENEWINDOW_H


#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <GameClient/Windows/GameSceneMenuGizmo.h>

class SceneWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<SceneWindow>();
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
        window->Show();
    }

#if UNITY_EDITOR
    sf::Vector3f position = {0, 0, 0}; // I'n World coordinates
    float orthographicSize = 1; //Zoom
    sf::RenderTexture texture;

    void Awake() override;

    void DrawCamera();

    void OnStyleChange() override;

    void OnStylePop() override;

#endif

    void OnGUI() override;

};

#endif //RTS_GAME_SCENEWINDOW_H
