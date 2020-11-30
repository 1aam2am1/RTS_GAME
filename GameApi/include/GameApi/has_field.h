//
// Created by Michal_Marszalek on 30.11.2020.
//

#ifndef RTS_GAME_HAS_FIELD_H
#define RTS_GAME_HAS_FIELD_H

#include <type_traits>

#define HAS_FIELD(FIELD) \
namespace{               \
template <typename, typename = void>    \
struct has_field##FIELD : std::false_type {};  \
                                        \
template <typename T>                   \
struct has_field##FIELD<T, std::void_t<decltype(&T::FIELD)>> : std::is_member_function_pointer<decltype(&T::FIELD)>  \
{};                      \
                         \
template <typename T>    \
inline constexpr bool has_field##FIELD##_v = has_field##FIELD<T>::value;  \
}

#endif //RTS_GAME_HAS_FIELD_H
