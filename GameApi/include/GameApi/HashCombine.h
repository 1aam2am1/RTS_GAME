//
// Created by Michal_Marszalek on 22.10.2020.
//

#ifndef RTS_GAME_HASHCOMBINE_H
#define RTS_GAME_HASHCOMBINE_H

#include <functional>

inline void hash_combine(std::size_t &) {}

template<typename T, typename... Rest>
inline void hash_combine(std::size_t &seed, const T &v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

#define MAKE_HASHABLE(type, ...) \
    namespace std {\
        template<> struct hash<type> {\
            std::size_t operator()(const type &t) const {\
                std::size_t ret = 0;\
                hash_combine(ret, __VA_ARGS__);\
                return ret;\
            }\
        };\
    }

#endif //RTS_GAME_HASHCOMBINE_H
