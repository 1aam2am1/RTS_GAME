//
// Created by Michal_Marszalek on 09.12.2020.
//

#ifndef RTS_GAME_GAMELOOP_H
#define RTS_GAME_GAMELOOP_H


#include <SFML/System/Clock.hpp>
#include <string>


class GameLoop {
public:
    void run();

private:
    sf::Clock deltaClock;
    float m_physics_time = 0;

    bool isPlaying = false;
    bool isPaused = false;
    std::string loaded_scene_path;
};


#endif //RTS_GAME_GAMELOOP_H
