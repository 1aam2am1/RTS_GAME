//
// Created by Michal_Marszalek on 09.09.2020.
//

#ifndef RTS_GAME_MONOBEHAVIOUR_H
#define RTS_GAME_MONOBEHAVIOUR_H

#include <GameApi/Signal.h>

template<typename, typename>
class TPtr;

class MonoBehaviour {
public:

    virtual ~MonoBehaviour() = default;

#ifndef TEST
    private:
#endif
    sigslot::signal<> onDestroySignal;

    template<typename, typename>
    friend
    class TPtr;
};


#endif //RTS_GAME_MONOBEHAVIOUR_H
