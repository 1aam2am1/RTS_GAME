//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_WILL_H
#define RTS_GAME_WILL_H


template<typename... Handlers>
struct Will : Handlers ... {
    using Handlers::handle...;
};


#endif //RTS_GAME_WILL_H
