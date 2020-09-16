//
// Created by Michal_Marszalek on 15.09.2020.
//

#ifndef RTS_GAME_OBJECT_H
#define RTS_GAME_OBJECT_H

#include <GameApi/Signal.h>

/// Base class for all objects Unity can reference.
class Object {
public:

    virtual ~Object() = default;

#ifndef TEST
private:
#endif

    sigslot::signal<> onDestroySignal;

    template<typename>
    friend
    class TPtr;
};


#endif //RTS_GAME_OBJECT_H
