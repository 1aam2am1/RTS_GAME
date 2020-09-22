//
// Created by Michal_Marszalek on 22.09.2020.
//

#include "BasicLayout.h"
#include <imgui.h>
#include <imgui_internal.h>

int globalDockId{};

void BasicLayout() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    globalDockId = ImGui::GetID("MyDockSpace");
    if (ImGui::DockBuilderGetNode(globalDockId) == nullptr) {
        ImGui::DockBuilderRemoveNode(globalDockId); // Clear out existing layout
        ImGui::DockBuilderAddNode(globalDockId, ImGuiDockNodeFlags_DockSpace); // Add empty node
        ImGui::DockBuilderSetNodeSize(globalDockId, viewport->Size);

        ImGuiID dock_main_id = globalDockId; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr,
                                                            &dock_main_id);
        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr,
                                                             &dock_main_id);

        ImGui::DockBuilderDockWindow("Hello, World", dock_id_left);
        ImGui::DockBuilderDockWindow("james", dock_main_id);
        ImGui::DockBuilderDockWindow("James_3", dock_id_right);
        ImGui::DockBuilderDockWindow("Hello, World2", dock_id_bottom);
        ImGui::DockBuilderFinish(globalDockId);
    }

    ImGui::DockSpace(globalDockId, ImVec2(0.0f, 0.0f), dockspace_flags);


    ImGui::End();
}
