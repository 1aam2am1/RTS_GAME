//
// Created by Michal_Marszalek on 23.09.2020.
//

#include "MainWindow.h"
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <GameClient/Unity/Editor/EditorWindow.h>


int globalDockId{};

auto BasicLayout = []() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;// | ImGuiDockNodeFlags_CentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    window_flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("GLOBAL_DOCK_SPACE", nullptr, window_flags);

    ImGui::PopStyleVar(3);

    if (ImGui::DockBuilderGetNode(globalDockId) == nullptr) {
        ImGui::DockBuilderRemoveNode(globalDockId); // Clear out existing layout
        ImGui::DockBuilderAddNode(globalDockId, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(globalDockId, viewport->Size);

        ImGuiID dock_main_id = globalDockId; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        // Inspector space
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr,
                                                            &dock_main_id);
        // Assets, Console space
        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.35f, nullptr,
                                                             &dock_main_id);
        // Hierarchy space
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.35f, nullptr,
                                                           &dock_main_id);

        ImGui::DockBuilderDockWindow("###Inspector", dock_id_right);
        ImGui::DockBuilderDockWindow("###Console", dock_id_bottom);
        ImGui::DockBuilderDockWindow("###Hierarchy", dock_id_left);
        ImGui::DockBuilderDockWindow("###Game", dock_main_id);

        ImGui::DockBuilderFinish(globalDockId);
    }

    ImGui::DockSpace(globalDockId, ImVec2(0.0f, 0.0f), dockspace_flags);


    ImGui::End();
};

MainWindow::MainWindow(const Argv_options &options) {
    window.create(sf::VideoMode{options.getOptions().size.x, options.getOptions().size.y}, "RTS_GAME",
                  sf::Style::Default, sf::ContextSettings{0, 0, 0, 2, 1});

    ImGui::SFML::Init(window);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::GetIO().IniFilename = nullptr;

    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

    window.setFramerateLimit(options.getOptions().fps);

    lastEvents.reserve(16);

    constexpr std::string_view dock_name = "###GLOBAL_DOCK";
    globalDockId = ImHashStr(dock_name.data(), dock_name.length(), 0);
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

        BasicLayout(); /// Set basic layout and global dock window

        for (const auto &p : EditorWindow::get_open_windows()) {
            p->drawGui();  /// drawing ImGui and events to Game Scene
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
    }
}
