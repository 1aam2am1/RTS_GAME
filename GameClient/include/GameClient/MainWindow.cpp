//
// Created by Michal_Marszalek on 23.09.2020.
//

#include "MainWindow.h"
#include "GarbageCollector.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>

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
}

MainWindow::~MainWindow() {
    ImGui::SFML::Shutdown();
}

//ImGui::ShowDemoWindow();
void MainWindow::run() {
    sf::Clock deltaClock;

    while (window.isOpen()) {
        /**
         * Phisics
         *
         */

        /**
         * Events
         */
        sf::Event event{};
        lastEvents.clear();
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event); /// Set global io events -> ImGui

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            lastEvents.push_back(event);
        }

        ImGui::SFML::Update(window, deltaClock.restart()); /// ImGui Io events eating

        WindowLayout::drawLayout(); /// Set basic layout and global dock window

        ///No for each as windows can change
        ///TODO: Change this as adding to different list and adding it on end after drawing all.
        for (size_t i = 0; i < EditorWindow::get_open_windows().size(); ++i) {
            EditorWindow::get_open_windows()[i]->Update();  /// drawing ImGui and events to Game Scene
        }

        for (size_t i = 0; i < EditorWindow::get_open_windows().size(); ++i) {
            EditorWindow::get_open_windows()[i]->drawGui();  /// drawing ImGui and events to Game Scene
        }

        /**
         * Game logic
         */

        /**
         * Render game scene to render texture
         */

        /**
         * Future onGui
         */

        window.clear();
        ImGui::SFML::Render(window);
        window.display();

        GarbageCollector::clear();
    }
}
