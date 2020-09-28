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
    Menu();

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Menu>();
        window->Show();
    }

    /// Add MenuItem and function
    /// \param path Path of item.
    /// \param priority The order by which the menu items are displayed.
    template<typename F>
    static void addItem(std::string path, const F &f, int priority = 0);

    static void addPlaceHolder(std::string path, int priority = 0);

    void OnGUI() override;

private:
    void drawGui() override;

    std::vector<MenuItem> &items;

    static void addItem(std::string path, std::function<bool(MenuCommand)> f, bool validation, int priority);
};

template<typename F>
void Menu::addItem(std::string path, const F &f, int priority) {
    if constexpr (std::is_convertible<F, void (*)()>::value) {
        addItem(path, [=](auto) -> bool {
            f();
            return true;
        }, false, priority);
    } else if constexpr (std::is_convertible<F, bool (*)()>::value) {
        addItem(path, [=](auto) -> bool {
            return f();
        }, true, priority);
    } else if constexpr (std::is_convertible<F, void (*)(MenuCommand)>::value) {
        addItem(path, [=](auto e) -> bool {
            f(e);
            return true;
        }, false, priority);
    } else if constexpr (std::is_convertible<F, bool (*)(MenuCommand)>::value) {
        addItem(path, f, true, priority);
    } else {
        static_assert(std::is_convertible<F, void (*)()>::value, "Wrong function type");
    }
}

#endif //RTS_GAME_MENU_H
