//
// Created by Michal_Marszalek on 28.09.2020.
//

#ifndef RTS_GAME_ASSETWINDOW_H
#define RTS_GAME_ASSETWINDOW_H


#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>

class AssetWindow : public EditorWindow {
public:
    static void Init();

    void Awake() override;

    void OnStyleChange() override;

    void OnStylePop() override;

    void OnGUI() override;

private:
    std::string search_string;

    std::vector<std::string> objects;
    std::string root;

    bool start = false;

    void display_tree(std::string path, std::string name);

    void display_files();
};


#endif //RTS_GAME_ASSETWINDOW_H
