//
// Created by Michal_Marszalek on 01.10.2020.
//

#ifndef RTS_GAME_GARBAGECOLLECTOR_H
#define RTS_GAME_GARBAGECOLLECTOR_H

#include <memory>
#include <GameClient/Unity/Core/Object.h>

class GarbageCollector {
public:

    static void add(std::shared_ptr<Object>, std::function<void(std::shared_ptr<Object>)> f = {});

    //TODO: In different thread memory release
    static void clear();

private:
};


#endif //RTS_GAME_GARBAGECOLLECTOR_H
