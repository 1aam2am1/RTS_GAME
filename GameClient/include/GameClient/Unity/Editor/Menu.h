//
// Created by Michal_Marszalek on 24.09.2020.
//

#ifndef RTS_GAME_MENU_H
#define RTS_GAME_MENU_H


#include "EditorWindow.h"
#include "MenuCommand.h"
#include "Macro.h"

struct MenuItem;

class Menu : public EditorWindow {
public:
    using Func = void (*)(MenuCommand);

    Menu();

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Menu>();
        window->Show();
    }

    /// Add MenuItem and function
    /// \param path Path of item.
    /// \param priority The order by which the menu items are displayed.
    static void addItem(std::string path, Func f, int priority = 0);

    static void addPlaceHolder(std::string path, int priority = 0);

    void OnGUI() override;

private:
    void drawGui() override;

    std::vector<MenuItem> &items;
};


#endif //RTS_GAME_MENU_H
