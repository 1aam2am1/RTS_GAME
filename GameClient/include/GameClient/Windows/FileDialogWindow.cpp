//
// Created by Michal_Marszalek on 09.01.2021.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <GameClient/IconsFontAwesome5_c.h>
#include "FileDialogWindow.h"

namespace fs = std::filesystem;

void FileDialogWindow::Init() {
    // Get existing open window or if none, make a new one:
    auto window = EditorWindow::GetWindow<FileDialogWindow>();
    window->minSize.x = 600;
    window->minSize.y = 500;
    window->ShowPopup();
}

void FileDialogWindow::Awake() {
    files_ls();
}

void FileDialogWindow::OnGUI() {
    if (ImGui::BeginTable("Header", 2, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, -1);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetWindowContentRegionWidth() * 0.3f);

        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();

        auto &style = ImGui::GetStyle();
        ImVec2 sw_size = ImVec2(0, style.WindowPadding.y * 2.0f + ImGui::GetFrameHeight());
        ImVec2 nw_size = ImVec2(0, sw_size.y);

        ImGui::BeginChild("Path", nw_size, true, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", path.data());
        ImGui::EndChild();

        ImGui::TableNextColumn();

        ImGui::BeginChild("Search", sw_size, true, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputTextWithHint("##Search", ICON_FA_SEARCH, &search);
        ImGui::EndChild();


        ImGui::EndTable();
    }

    ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 pw_size = ImGui::GetWindowSize();
    float input_bar_ypos = pw_size.y - ImGui::GetFrameHeightWithSpacing() * 2.5f - style.WindowPadding.y;
    float window_height = input_bar_ypos - ImGui::GetCursorPosY() - style.ItemSpacing.y;

    bool clicked_dir = false;

    ImGui::BeginChild("Directories", {0, window_height}, true, ImGuiWindowFlags_NoDecoration);
    {
        float list_item_height = ImGui::CalcTextSize("").y + style.ItemSpacing.y;
        float window_content_height = window_height - style.WindowPadding.y * 2.0f;
        int col_items_limit = static_cast<int>(std::max(1.0f, window_content_height / list_item_height));

        int num_cols = static_cast<int>(std::max(1.0f, std::ceil(
                static_cast<float>(filtered_dirs.size() + filtered_files.size()) / col_items_limit)));
        if (num_cols > 64) {
            int exceed_items_amount = (num_cols - 64) * col_items_limit;
            col_items_limit += static_cast<int>(std::ceil(exceed_items_amount / 64.0));
            num_cols = static_cast<int>(std::max(1.0f, std::ceil(
                    static_cast<float>(filtered_dirs.size() + filtered_files.size()) / col_items_limit)));
        }

        if (ImGui::BeginTable("Header", num_cols, ImGuiTableFlags_BordersInnerV)) {
            for (int64_t i = 0; i < num_cols; ++i) {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 280);
            }

            for (auto &it : filtered_dirs) {
                ImGui::TableNextColumn();

                if (ImGui::Selectable((ICON_FA_FOLDER " " + it).c_str(), selected == it,
                                      ImGuiSelectableFlags_AllowDoubleClick)) {

                    selected = it;
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        clicked_dir = true;
                        path += "/" + it;
                    }
                }
            }

            if (m_new_folder_selectable) {
                ImGui::TableNextColumn();

                if (!ImGui::IsAnyItemActive()) { ImGui::SetKeyboardFocusHere(); }
                if (ImGui::InputText("##new_dir", &selected, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    path += "/" + selected;
                    clicked_dir = true;

                    fs::create_directory(path);

                    m_new_folder_selectable = false;
                }
                if (ImGui::IsItemDeactivated()) {
                    m_new_folder_selectable = false;
                }
            }

            for (auto &it : filtered_files) {
                ImGui::TableNextColumn();

                if (ImGui::Selectable((ICON_FA_FILE " " + it).c_str(), selected == it,
                                      ImGuiSelectableFlags_AllowDoubleClick)) {

                    selected = it;
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        if (execute) {
                            execute(fs::path(path + "/" + selected));
                            execute = [](auto) {};
                        }
                        Close();
                    }
                }
            }

            ImGui::EndTable();
        }

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) {
            ImGui::OpenPopup("##Create_dir");
        }

        if (ImGui::BeginPopup("##Create_dir")) {
            if (ImGui::BeginMenu("New")) {
                if (ImGui::MenuItem("Folder")) {
                    m_new_folder_selectable = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

    }
    ImGui::EndChild();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Open:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputTextWithHint("##File_name", "Type a name...", &selected);

    if (ImGui::Checkbox("Show Hidden Files and Folders", &hidden)) {
        files_ls();
    }
    ImGui::SameLine();

    float pos = ImGui::GetWindowContentRegionWidth()
                - ImGui::CalcTextSize("Open").x
                - ImGui::CalcTextSize("Cancel").x
                - ImGui::GetStyle().FramePadding.x * 2.0
                - ImGui::GetStyle().WindowPadding.x;

    ImGui::SetCursorPosX(pos);
    bool open = ImGui::Button("Open");
    ImGui::SameLine();
    bool close = ImGui::Button("Cancel");

    if (open && execute) {
        execute(fs::path(path + "/" + selected));
    }

    if (open || close) {
        Close();
    }

    if (clicked_dir) {
        files_ls();
    }
}

void FileDialogWindow::files_ls() {
    files.clear();
    dirs.clear();
    selected.clear();

    path = fs::path(path).lexically_normal().generic_string();
    if (!path.starts_with("Assets")) { path = "Assets/"; }
    if (path.size() > 7) {
        dirs.emplace_back("..");
    }

    for (auto &p: fs::directory_iterator(path, fs::directory_options::skip_permission_denied)) {
        if (p.path().extension() == ".meta") { continue; }
        if (p.is_regular_file()) {
            files.emplace_back(p.path().filename().string());
        } else if (p.is_directory()) {
            dirs.emplace_back(p.path().filename().string());
        }
    }

    filtered_files.clear();
    filtered_dirs.clear();

    filtered_files = files;
    filtered_dirs = dirs;
}

MENU_ITEM(FileDialogWindow::Init, "Help/File dialog");
