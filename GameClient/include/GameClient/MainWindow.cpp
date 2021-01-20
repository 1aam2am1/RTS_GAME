//
// Created by Michal_Marszalek on 23.09.2020.
//

#include "MainWindow.h"
#include "MainThread.h"
#include "GlobalStaticVariables.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/Unity/Core/Application.h>
#include <GameClient/WindowLayout.h>
#include <Core/MonoBehaviour.h>
#include <Core/Attributes.h>

#if !(UNITY_EDITOR)
#include <GameClient/GlobalStaticVariables.h>
#endif

MainWindow::MainWindow(const Argv_options &options) {
    window.create(sf::VideoMode{options.getOptions().size.x, options.getOptions().size.y}, "RTS_GAME",
                  sf::Style::Default, sf::ContextSettings{0, 0, 0, 2, 1});

    ImGui::SFML::Init(window);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::GetIO().IniFilename = nullptr;

    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;
    ImGui::GetStyle().WindowRounding = 0;
    ImGui::GetStyle().TabRounding = 0;

    window.setFramerateLimit(options.getOptions().fps);

    lastEvents.reserve(16);

#if !(UNITY_EDITOR)
    global.m_real_target = &window;
#endif
    global.rendering.m_window = &window;
}

MainWindow::~MainWindow() {
    ImGui::SFML::Shutdown();
}

void MainWindow::run() {
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event{};
        lastEvents.clear();
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event); /// Set global io events -> ImGui

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            lastEvents.emplace_back(event);
        }

        window.clear();

        ImGui::SFML::Update(window, deltaClock.restart()); /// ImGui Io events eating

        WindowLayout::drawLayout(); /// Set basic layout and global dock window

        game.run(); /// Before editors because this changes Time.deltaTime

        ///No for each as windows can change
        ///TODO: Change this as adding to different list and adding it on end after drawing all.
        ///TODO: Current implementation has iterator leak when get_open_windows resizing.
        {
            auto &windows = EditorWindow::get_open_windows();

            for (auto it = windows.begin(); it != windows.end();) {
                if (*it) {
                    (*it)->Update();  /// drawing ImGui and events to Game Scene
                    ++it;
                } else {
                    it = windows.erase(it);
                }
            }

            for (auto it = windows.begin(); it != windows.end();) {
                if (*it) {
                    (*it)->drawGui();  /// drawing ImGui and events to Game Scene
                    ++it;
                } else {
                    it = windows.erase(it);
                }
            }
        }

        {
            ///On Gui TODO: Change this for another context (context in context), do the same for ImGui Components
            if (!global.rendering.m_game_imGuiName.empty()) {
                auto w = ImGui::FindWindowByName(global.rendering.m_game_imGuiName.data());
                if (w && w->Active) {
                    if (ImGui::Begin(global.rendering.m_game_imGuiName.data())) {
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

                    ImGui::End();
                }
            }


            ///Wait for end of frame
        }

        ImGui::SFML::Render(window);
        window.display();

        ///Run invoked functions on main thread
        MainThread::run();
    }
}
