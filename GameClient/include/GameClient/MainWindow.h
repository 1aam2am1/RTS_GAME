//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_MAINWINDOW_H
#define RTS_GAME_MAINWINDOW_H

#include "Argv_options.h"
#include <memory>
#include <vector>
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>

class EditorWindow;

class GameWindow;

class MainWindow {
public:
    explicit MainWindow(const Argv_options &options);

    ~MainWindow();

    static void addPanel(const std::shared_ptr<EditorWindow> &);

    void run();

private:
    sf::RenderWindow window;

    static std::list<std::shared_ptr<EditorWindow>> list;

private:
    std::vector<sf::Event> lastEvents;

    friend GameWindow;
};


#endif //RTS_GAME_MAINWINDOW_H
