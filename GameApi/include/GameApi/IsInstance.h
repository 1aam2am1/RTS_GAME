//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_ISINSTANCE_H
#define RTS_GAME_ISINSTANCE_H

#include <type_traits>

namespace {
    template<typename, template<typename...> typename>
    struct is_instance_impl : public std::false_type {
    };

    template<template<typename...> typename U, typename...Ts>
    struct is_instance_impl<U<Ts...>, U> : public std::true_type {
    };
}

template<typename T, template<typename ...> typename U>
using is_instance = is_instance_impl<std::decay_t<T>, U>;

template<typename T, template<typename ...> typename U>
inline constexpr bool is_instance_v = is_instance<T, U>::value;


#endif //RTS_GAME_ISINSTANCE_H
