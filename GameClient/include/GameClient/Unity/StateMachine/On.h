//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_ON_H
#define RTS_GAME_ON_H


template<typename Event, typename Action>
struct On {
    Action handle(const Event &) const {
        return {};
    }
};


#endif //RTS_GAME_ON_H
