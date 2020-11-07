//
// Created by Michal_Marszalek on 30.10.2020.
//

#ifndef RTS_GAME_INITIALIZER_H
#define RTS_GAME_INITIALIZER_H

#include <functional>

class Initializer {
public:
    static int add(std::function<void()> f);

    static void initialize();

    static int d_add(std::function<void()> f);

    static void uninitialize();
};


#endif //RTS_GAME_INITIALIZER_H
