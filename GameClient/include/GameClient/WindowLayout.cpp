//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "WindowLayout.h"
#include <imgui.h>
#include <imgui_internal.h>

constexpr std::string_view dock_name = "###GLOBAL_DOCK";
ImU32 globalDockId = ImHashStr(dock_name.data(), dock_name.length(), 0);

/// = {dock_id_left, dock_id_right, dock_id_center, dock_id_up, dock_id_bottom};
ImGuiID dock[5] = {globalDockId, globalDockId, globalDockId, globalDockId, globalDockId};

void WindowLayout::dockWindow(WindowLayout::Align align, std::shared_ptr<EditorWindow> ptr) {
    if (align > 5) { align = Align::Center; }
    return;
    //TODO: Docking not working repair this.
    if (ImGuiWindow *window = ImGui::FindWindowByName(("###" + GameApi::to_string(ptr.get())).c_str())) {
        auto node = ImGui::DockBuilderGetNode(dock[align]);
        //TODO: if window->DockNode then dock node to main_node
        //TODO: if node == nullptr create node and then dock node
        if (window->DockId != dock[align] && node != nullptr && !node->IsFloatingNode())
            ImGui::DockContextQueueDock(ImGui::GetCurrentContext(), nullptr, node,
                                        window,
                                        ImGuiDir_None, 0.f, false);
    } else {
        auto node = ImGui::DockBuilderGetNode(dock[align]);
        if (node)
            ImGui::DockBuilderDockWindow(("###" + GameApi::to_string(ptr.get())).c_str(), dock[align]);
    }
    //TODO: DockBuilderDockWindow don't work on existing windows therefore it is not possible to dock
    // existing window to node that could be in unusable state.
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
        dock[1] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr,
                                              &dock_main_id);
        // Assets, Console space
        dock[4] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.35f, nullptr,
                                              &dock_main_id);
        // Hierarchy space
        dock[0] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.35f, nullptr,
                                              &dock_main_id);

        dock[2] = dock_main_id;
        dock[3] = dock_main_id;

        ImGui::DockBuilderFinish(globalDockId);
    }

    ImGui::DockSpace(globalDockId, ImVec2(0.0f, 0.0f), dockspace_flags);


    ImGui::End();
}
