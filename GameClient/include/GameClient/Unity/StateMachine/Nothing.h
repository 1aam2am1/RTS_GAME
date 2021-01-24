//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_NOTHING_H
#define RTS_GAME_NOTHING_H


struct Nothing {
    template<typename Machine, typename State, typename Event>
    void execute(Machine &, State &, const Event &) {
    }
};


#endif //RTS_GAME_NOTHING_H
