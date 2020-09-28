//
// Created by Michal_Marszalek on 28.09.2020.
//

#ifndef RTS_GAME_ASSETWINDOW_H
#define RTS_GAME_ASSETWINDOW_H


#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>

class AssetWindow : public EditorWindow {
public:
    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<AssetWindow>();
        WindowLayout::dockWindow(WindowLayout::Down, window);
        window->Show();
    }

    void OnGUI() override {

    }
};


#endif //RTS_GAME_ASSETWINDOW_H
