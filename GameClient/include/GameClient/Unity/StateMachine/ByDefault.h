//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_BYDEFAULT_H
#define RTS_GAME_BYDEFAULT_H


template<typename Action>
struct ByDefault {
    ByDefault() = default;

    template<typename Event>
    Action handle(const Event &) const {
        return Action{};
    }
};


#endif //RTS_GAME_BYDEFAULT_H
