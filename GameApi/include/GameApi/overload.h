//
// Created by Michal_Marszalek on 14.09.2020.
//

#ifndef RTS_GAME_OVERLOAD_H
#define RTS_GAME_OVERLOAD_H

#include <variant>

/// Helper for std::visit
template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};
template<class... Ts> overload(Ts...) -> overload<Ts...>;

template<class... Args>
struct variant_cast_proxy {
    std::variant<Args...> v;

    template<class... ToArgs>
    operator std::variant<ToArgs...>() const {
        return std::visit([](auto &&arg) -> std::variant<ToArgs...> { return arg; },
                          v);
    }
};

template<class... Args>
auto variant_cast(const std::variant<Args...> &v) -> variant_cast_proxy<Args...> {
    return {v};
}

#endif //RTS_GAME_OVERLOAD_H
