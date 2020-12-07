//
// Created by Michal_Marszalek on 15.08.2020.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"
#ifndef RTS_GAME_REVERSE_H
#define RTS_GAME_REVERSE_H

#include <vector>

//#if __cplusplus <= 201703L

template<typename T>
class reverse {
private:
    const T &iterable;
public:
    constexpr explicit reverse(T &iterable) noexcept: iterable{iterable} {}

    constexpr auto begin() const noexcept { return std::rbegin(iterable); }

    constexpr auto end() const noexcept { return std::rend(iterable); }
};

//#endif

#endif //RTS_GAME_REVERSE_H

#pragma clang diagnostic pop
