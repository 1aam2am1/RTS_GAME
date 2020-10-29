//
// Created by Michal_Marszalek on 30.10.2020.
//

#ifndef RTS_GAME_FUNCTION_TRAITS_H
#define RTS_GAME_FUNCTION_TRAITS_H

#include <functional>

namespace {
    template<typename T>
    struct function_traits_impl;

    template<typename R, typename ...Args>
    struct function_traits_impl<std::function<R(Args...)>> {
        static constexpr size_t nargs = sizeof...(Args);

        typedef R result_type;

        template<size_t i>
        using arg = typename std::tuple_element<i, std::tuple<Args...>>;

        template<size_t i>
        using arg_t = typename arg<i>::type;
    };
}

template<typename T>
using function_traits = function_traits_impl<std::decay_t<T>>;

template<typename T, size_t i>
using function_traits_arg_t = typename function_traits<T>::template arg_t<i>;


#endif //RTS_GAME_FUNCTION_TRAITS_H
