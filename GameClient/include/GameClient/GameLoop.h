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

    struct {
        uint64_t old_id = 0;
        std::string name{};
        std::string path{};
    } old_scene;
};


#endif //RTS_GAME_GAMELOOP_H
