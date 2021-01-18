//
// Created by Michal_Marszalek on 2021-01-17.
//

#ifndef RTS_GAME_ISBASEOFTEMPLATE_H
#define RTS_GAME_ISBASEOFTEMPLATE_H

template<template<typename...> class C, typename...Ts>
std::true_type is_base_of_template_impl(const C<Ts...> *);

template<template<typename...> class C>
std::false_type is_base_of_template_impl(...);

template<typename T, template<typename...> class C>
using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T *>()));

template<typename T, template<typename...> class C>
inline constexpr bool is_base_of_template_v = is_base_of_template<T, C>::value;

#endif //RTS_GAME_ISBASEOFTEMPLATE_H
