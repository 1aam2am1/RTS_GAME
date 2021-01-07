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
#include <filesystem>
#include <GameClient/Unity/Editor/Selection.h>
#include <Editor/DragAndDrop.h>

MENU_ITEM(AssetWindow::Init, "Window/General/Project", 5)

namespace fs = std::filesystem;

std::vector<std::string> paths(std::string path) {
    std::vector<std::string> result;
    result.reserve(32);

    for (auto &p: fs::directory_iterator(path, fs::directory_options::skip_permission_denied)) {
        if (p.is_regular_file() && p.path().extension() != ".meta") {
            result.emplace_back(p.path().filename().string());
        }
    }

    return result;
}

void AssetWindow::Init() {
    // Get existing open window or if none, make a new one:
    auto window = EditorWindow::GetWindow<AssetWindow>();
    WindowLayout::dockWindow(WindowLayout::Down, window);
    window->minSize.x = 500;
    window->flags |= ImGuiWindowFlags_MenuBar;
    window->Show();
}

void AssetWindow::Awake() {
    titleContent = "Project";
    root = "Assets";
    objects = paths("Assets");
}

void AssetWindow::OnStyleChange() {
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
}

void AssetWindow::OnStylePop() {
    //ImGui::PopStyleVar(2);
}

void AssetWindow::OnGUI() {
    if (ImGui::BeginMenuBar()) {
        ImGui::SetCursorPosX(0);
        ImGui::Button(ICON_FA_PLUS " " ICON_FA_ANGLE_DOWN);

        float LocalButtonWidth = 200.0f;
        auto pos = ImGui::GetWindowWidth() - LocalButtonWidth - 2;
        if (pos < ImGui::GetCursorPosX()) {
            pos = ImGui::GetCursorPosX();
            LocalButtonWidth = ImGui::GetWindowWidth() - pos - 2;
        }

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
        display_tree("Assets", "Assets");
    }
    ImGui::EndChild();

    ImGui::NextColumn();

    display_files();
}

void AssetWindow::display_tree(std::string path, std::string name) {
    const auto d = AssetDatabase::GetSubFolders(path);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

    if (path == "Assets") {
        flags |= ImGuiTreeNodeFlags_DefaultOpen;
    }

    bool open = ImGui::TreeNodeEx(name.data(), flags);
    bool clicked = ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();

    if (open) {
        for (const auto &it : d) {
            display_tree(path + "/" + it.data(), it.data());
        }

        ImGui::TreePop();
    }

    if (clicked) {
        root = path;
        objects = paths(root);
    }
}

void AssetWindow::display_files() {
    ImGui::BeginChild("ChildR", {0, 0}, false, ImGuiWindowFlags_NoDecoration);

    ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 button_sz(120, 40);

    float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    for (size_t n = 0; n < objects.size(); n++) {

        bool clicked = ImGui::Button(objects[n].data(), button_sz);
        bool double_clicked = ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered();


        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0) && !DragAndDrop::IsDragging()) {
            DragAndDrop::PrepareStartDrag();

            auto main = AssetDatabase::LoadMainAssetAtPath(root + "/" + objects[n]);
            if (main) { DragAndDrop::SetGenericData("OBJECT", main); }
            DragAndDrop::paths.emplace_back(root + "/" + objects[n]);

            DragAndDrop::StartDrag(objects[n]);
        }

        if (clicked) {
            auto guid = AssetDatabase::AssetPathToGUID(root + "/" + objects[n]);
            auto main = AssetDatabase::LoadMainAssetAtPath(root + "/" + objects[n]);

            Selection::assetGUIDs.clear();
            Selection::assetGUIDs.emplace_back(guid);

            Selection::activeGameObject = dynamic_pointer_cast<GameObject>(main);
            if (Selection::activeGameObject) {
                Selection::activeTransform = Selection::activeGameObject->transform();
            } else {
                Selection::activeTransform = nullptr;
            }
            Selection::activeObject = main;
        }

        if (double_clicked) {
            AssetDatabase::OpenAsset(AssetDatabase::LoadMainAssetAtPath(root + "/" + objects[n]));
        }

        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 = last_button_x2 + style.ItemSpacing.x +
                               button_sz.x; // Expected position if next button was on same line
        if (n + 1 < objects.size() && next_button_x2 < window_visible_x2)
            ImGui::SameLine();
    }

    ImGui::EndChild();
}

