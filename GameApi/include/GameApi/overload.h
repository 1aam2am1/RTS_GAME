//
// Created by Michal_Marszalek on 14.09.2020.
//

#ifndef RTS_GAME_OVERLOAD_H
#define RTS_GAME_OVERLOAD_H

/// Helper for std::visit
template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...) -> overload<Ts...>;

#endif //RTS_GAME_OVERLOAD_H
