//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "AssetWindow.h"
#include "Macro.h"
#include <Editor/Menu.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include "GameClient/IconsFontAwesome5_c.h"

MENU_ITEM(AssetWindow::Init, "Window/General/Project", 5)

void AssetWindow::Init() {
    // Get existing open window or if none, make a new one:
    auto window = EditorWindow::GetWindow<AssetWindow>();
    WindowLayout::dockWindow(WindowLayout::Down, window);
    window->flags |= ImGuiWindowFlags_MenuBar;
    window->Show();
}

void AssetWindow::Awake() {
    titleContent = "Project";
}

void AssetWindow::OnStyleChange() {
//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
}

void AssetWindow::OnStylePop() {
//ImGui::PopStyleVar(1);
}

void AssetWindow::OnGUI() {
    if (ImGui::BeginMenuBar()) {
        ImGui::SetCursorPosX(0);
        ImGui::Button(ICON_FA_PLUS " " ICON_FA_ANGLE_DOWN);

        const float ItemSpacing = ImGui::GetStyle().ItemSpacing.x;

        static float LocalButtonWidth = 200.0f;
        auto pos = ImGui::GetWindowWidth() - (LocalButtonWidth + ItemSpacing);
        if (pos < ImGui::GetCursorPosX()) { pos = ImGui::GetCursorPosX(); }
        ImGui::SetCursorPosX(pos);
        ImGui::SetNextItemWidth(LocalButtonWidth);
        ImGui::InputTextWithHint("##Search", ICON_FA_SEARCH, &search_string);

        ImGui::EndMenuBar();
    }

    ImGui::Columns(2, nullptr, true);
    if (!start) {
        ImGui::SetColumnWidth(-1, ImGui::GetWindowContentRegionWidth() * 0.3f);
        start = true;
    }

    if (ImGui::BeginChild("Tree", {0, 0}, false, ImGuiWindowFlags_NoDecoration)) {
        display_tree({});
    }
    ImGui::EndChild();

    ImGui::NextColumn();

    if (search_string.empty()) {
        display_files();
    } else {
        display_search();
    }
}

void AssetWindow::display_tree(std::string path) {
    const auto d = AssetDatabase::GetSubFolders(path);

    for (const auto &it : d) {
        if (ImGui::TreeNode(it.data())) {
            display_tree(path + "/" + it.data());
            ImGui::TreePop();
        }
    }
}

void AssetWindow::display_files() {

}

void AssetWindow::display_search() {

}
