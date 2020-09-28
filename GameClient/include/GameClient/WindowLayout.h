//
// Created by Michal_Marszalek on 28.09.2020.
//

#ifndef RTS_GAME_WINDOWLAYOUT_H
#define RTS_GAME_WINDOWLAYOUT_H


#include <GameClient/Unity/Editor/EditorWindow.h>

class MainWindow;

class WindowLayout {
public:
    enum Align {
        Left = 0,
        Right,
        Center,
        Up,
        Down,
    };

    static void dockWindow(Align align, std::shared_ptr<EditorWindow> window);

private:
    friend class MainWindow;

    static void drawLayout();
};


#endif //RTS_GAME_WINDOWLAYOUT_H
