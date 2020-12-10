//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "WindowLayout.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imconfig.h>
#include <GameApi/reverse.h>
#include <GameClient/IconsFontAwesome5_c.h>
#include <Editor/EditorApplication.h>

constexpr std::string_view dock_name = "###GLOBAL_DOCK";
ImGuiID globalDockId = ImHashStr(dock_name.data(), dock_name.length(), 0);
static std::vector<std::string> place[5];

void WindowLayout::dockWindow(WindowLayout::Align align, TPtr<EditorWindow> ptr) {
    if (align > 5) { align = Align::Center; }
    if (!ptr) { return; }

    auto globalNode = ImGui::DockBuilderGetNode(globalDockId);
    if (!globalNode) {
        place[align].emplace_back(ptr->imGuiName);
        return;
    }

    std::list<WindowLayout::Align> old; //i'm from * site
    std::function<ImGuiID(ImGuiDockNode *, WindowLayout::Align)> get_node =
            [&get_node, &old](ImGuiDockNode *globalNode, WindowLayout::Align align) -> ImGuiID {
                IM_ASSERT(globalNode != nullptr);

                if (globalNode->IsSplitNode()) {
                    IM_ASSERT(globalNode->ChildNodes[0]);
                    IM_ASSERT(globalNode->ChildNodes[1]);

                    if (globalNode->SplitAxis == ImGuiAxis_X) {
                        auto split_0 = globalNode->ChildNodes[0]->IsSplitNode();
                        auto split_1 = globalNode->ChildNodes[1]->IsSplitNode();
                        switch (align) {
                            case Left: {
                                if (!split_0) { return globalNode->ChildNodes[0]->ID; }

                                old.emplace_back(Right);
                                return get_node(globalNode->ChildNodes[0], Left);
                            }
                            case Right: {
                                if (!split_1) { return globalNode->ChildNodes[1]->ID; }

                                old.emplace_back(Left);
                                return get_node(globalNode->ChildNodes[1], Right);
                            }
                            case Up: {
                                if (split_0) {
                                    old.emplace_back(Right);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Left);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ID, ImGuiDir_Up, 0.25f, nullptr,
                                                                       nullptr);
                                } else {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                            }
                            case Down: {
                                if (split_0) {
                                    old.emplace_back(Right);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Left);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ID, ImGuiDir_Down, 0.25f, nullptr,
                                                                       nullptr);
                                } else {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                            }
                            case Center: {
                                if (split_0) {
                                    old.emplace_back(Right);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Left);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Right,
                                                                       0.25f,
                                                                       nullptr,
                                                                       nullptr);
                                }
                                if (old.back() == Left) {
                                    return globalNode->ChildNodes[1]->ID;
                                }
                                if (old.back() == Right) {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                                for (auto c : reverse(old)) {
                                    if (c == Left) {
                                        return globalNode->ChildNodes[0]->ID;
                                    }
                                    if (c == Right) {
                                        return globalNode->ChildNodes[1]->ID;
                                    }
                                }
                                return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Right,
                                                                   0.25f,
                                                                   nullptr,
                                                                   nullptr);
                            }
                        }
                    } else { //y
                        auto split_0 = globalNode->ChildNodes[0]->IsSplitNode();
                        auto split_1 = globalNode->ChildNodes[1]->IsSplitNode();
                        switch (align) {
                            case Up: {
                                if (!split_0) { return globalNode->ChildNodes[0]->ID; }

                                old.emplace_back(Down);
                                return get_node(globalNode->ChildNodes[0], Up);
                            }
                            case Down: {
                                if (!split_1) { return globalNode->ChildNodes[1]->ID; }

                                old.emplace_back(Up);
                                return get_node(globalNode->ChildNodes[1], Down);
                            }
                            case Left: {
                                if (split_0) {
                                    old.emplace_back(Down);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Up);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ID, ImGuiDir_Left, 0.25f, nullptr,
                                                                       nullptr);
                                } else {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                            }
                            case Right: {
                                if (split_0) {
                                    old.emplace_back(Down);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Up);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ID, ImGuiDir_Right, 0.25f, nullptr,
                                                                       nullptr);
                                } else {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                            }
                            case Center: {
                                if (split_0) {
                                    old.emplace_back(Down);
                                    return get_node(globalNode->ChildNodes[0], align);
                                }
                                if (split_1) {
                                    old.emplace_back(Up);
                                    return get_node(globalNode->ChildNodes[1], align);
                                }
                                if (old.empty()) {
                                    return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Down,
                                                                       0.25f,
                                                                       nullptr,
                                                                       nullptr);
                                }
                                if (old.back() == Up) {
                                    return globalNode->ChildNodes[1]->ID;
                                }
                                if (old.back() == Down) {
                                    return globalNode->ChildNodes[0]->ID;
                                }
                                for (auto c : reverse(old)) {
                                    if (c == Up) {
                                        return globalNode->ChildNodes[0]->ID;
                                    }
                                    if (c == Down) {
                                        return globalNode->ChildNodes[1]->ID;
                                    }
                                }
                                return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Down,
                                                                   0.25f,
                                                                   nullptr,
                                                                   nullptr);
                            }
                        }
                    }
                } else if (!old.empty()) {
                    return globalNode->ID;
                } else {
                    switch (align) {
                        case Left:
                            return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Left,
                                                               0.25f,
                                                               nullptr,
                                                               nullptr);
                        case Right:
                            return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Right,
                                                               0.25f,
                                                               nullptr,
                                                               nullptr);
                        case Up:
                            return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Up,
                                                               0.25f,
                                                               nullptr,
                                                               nullptr);
                        case Down:
                            return ImGui::DockBuilderSplitNode(globalNode->ChildNodes[0]->ID, ImGuiDir_Down,
                                                               0.25f,
                                                               nullptr,
                                                               nullptr);
                        case Center:
                            return globalNode->ID;
                    }
                }

                IM_ASSERT(false);//< It won't happen
                return 0;
            };

    auto node = get_node(globalNode, align);

    ImGui::DockBuilderDockWindow(ptr->imGuiName.data(), node);
    ImGui::DockBuilderFinish(0);
}


void WindowLayout::drawLayout() {
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

#if UNITY_EDITOR
    window_flags |= ImGuiWindowFlags_MenuBar;
#endif
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("GLOBAL_DOCK_SPACE", nullptr, window_flags);

    ImGui::PopStyleVar(3);

#if UNITY_EDITOR
    if (ImGui::BeginMenuBar()) {
        auto pos = ImGui::GetWindowWidth() - 3 * 35;
        pos /= 2.0f;

        ImVec2 LocalButtonSize = {35, 19};
        if (pos < ImGui::GetCursorPosX()) {
            pos = ImGui::GetCursorPosX();
            LocalButtonSize = {0, 0};
        }

        ImGui::SetCursorPosX(pos);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        auto disabled_push = []() {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        };

        auto disabled_pop = []() {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        };

        auto enabled_push = []() {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        };

        auto enabled_pop = []() {
            ImGui::PopStyleColor();
        };

        {
            bool playing = EditorApplication::isPlaying;
            if (playing) { enabled_push(); }

            if (ImGui::Button(ICON_FA_PLAY "##play", LocalButtonSize)) {
                EditorApplication::isPlaying = !EditorApplication::isPlaying;
            }

            if (playing) { enabled_pop(); }
        }
        {
            bool paused = EditorApplication::isPaused;
            if (paused) { enabled_push(); }

            if (ImGui::Button(ICON_FA_PAUSE "##pause", LocalButtonSize)) {
                EditorApplication::isPaused = !EditorApplication::isPaused;
            }


            if (paused) { enabled_pop(); }
        }
        {
            disabled_push();
            ImGui::Button(ICON_FA_STEP_FORWARD "##step", LocalButtonSize);
            disabled_pop();
        }

        ImGui::PopStyleVar(2);

        ImGui::EndMenuBar();
    }
#endif

    if (ImGui::DockBuilderGetNode(globalDockId) == nullptr) {
        ImGui::DockBuilderRemoveNode(globalDockId); // Clear out existing layout
        ImGui::DockBuilderAddNode(globalDockId, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(globalDockId, viewport->Size);

        ImGuiID dock_main_id = globalDockId; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        // Inspector space
        auto right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr,
                                                 &dock_main_id);
        for (auto &window : place[Right]) {
            ImGui::DockBuilderDockWindow(window.data(), right);
        }

        // Assets, Console space
        auto down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.35f, nullptr,
                                                &dock_main_id);

        for (auto &window : place[Down]) {
            ImGui::DockBuilderDockWindow(window.data(), down);
        }

        // Hierarchy space
        auto left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.35f, nullptr,
                                                &dock_main_id);

        for (auto &window : place[Left]) {
            ImGui::DockBuilderDockWindow(window.data(), left);
        }

        for (auto &window : place[Up]) {
            ImGui::DockBuilderDockWindow(window.data(), dock_main_id);
        }
        for (auto &window : place[Center]) {
            ImGui::DockBuilderDockWindow(window.data(), dock_main_id);
        }

        ImGui::DockBuilderFinish(globalDockId);
    }

    ImGui::DockSpace(globalDockId, ImVec2(0.0f, 0.0f), dockspace_flags);


    ImGui::End();
}
