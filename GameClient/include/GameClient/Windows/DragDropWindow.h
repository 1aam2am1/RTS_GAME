//
// Created by Michal_Marszalek on 07.12.2020.
//

#ifndef RTS_GAME_DRAGDROPWINDOW_H
#define RTS_GAME_DRAGDROPWINDOW_H

#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"

class DragDropWindow : public EditorWindow {
public:

    static auto Init(std::string_view title) {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<DragDropWindow>();
        window->titleContent = title;
        window->Show();
        return window;
    }

    void OnGUI() override;

private:
    void drawGui() override;
};


#endif //RTS_GAME_DRAGDROPWINDOW_H
