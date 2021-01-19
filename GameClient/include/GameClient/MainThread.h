//
// Created by Michal_Marszalek on 01.10.2020.
//

#ifndef RTS_GAME_MAINTHREAD_H
#define RTS_GAME_MAINTHREAD_H

#include <functional>

class MainThread {
public:

    static void Invoke(std::function<void()> f);

private:
    friend class MainWindow;

    //TODO: In different thread memory release
    static void run();
};


#endif //RTS_GAME_MAINTHREAD_H
