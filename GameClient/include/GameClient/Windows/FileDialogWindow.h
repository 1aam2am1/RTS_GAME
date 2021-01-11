//
// Created by Michal_Marszalek on 09.01.2021.
//

#ifndef RTS_GAME_FILEDIALOGWINDOW_H
#define RTS_GAME_FILEDIALOGWINDOW_H


#include <Editor/EditorWindow.h>
#include <filesystem>

class FileDialogWindow : public EditorWindow {
public:
    static void Init();

    void Awake() override;

    void OnGUI() override;

    std::function<void(std::filesystem::path)> execute{};
private:
    std::string path = "Assets/";
    std::vector<std::string> files{};
    std::vector<std::string> dirs{};
    std::vector<std::string> filtered_files{};
    std::vector<std::string> filtered_dirs{};


    std::string search{};
    std::string selected{};
    bool hidden = false;
    bool m_new_folder_selectable = false;

    void files_ls();
};


#endif //RTS_GAME_FILEDIALOGWINDOW_H
