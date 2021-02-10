//
// Created by Michal_Marszalek on 13.01.2021.
//

#ifndef RTS_GAME_ATTRIBUTES_H
#define RTS_GAME_ATTRIBUTES_H

#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Object.h>

enum UnityAttributes {
    ExecuteInEditMode = 1 << 0,
    DisallowMultipleComponent = 1 << 1, //TODO: Make it inheritable
    DontNetworkSynchronize = 1 << 2
};

class Attributes {
public:
    template<typename T, typename E>
    static constexpr bool CheckCustomAttribute(const TPtr<T> &o, E e) {
        return CheckCustomAttribute(o.get(), e);
    }

    template<typename E>
    requires std::is_enum_v<E>
    static bool CheckCustomAttribute(const Object *o, E e) {
        if (!o) [[unlikely]] { return false; }
        if (o->flags) [[likely]] { return (*o->flags) & e; }
        const_cast<Object *>(o)->flags = GetAttribute(o);
        return (*o->flags) & e;
    }

private:
    static decltype(Object::flags) GetAttribute(const Object *);
};

#endif //RTS_GAME_ATTRIBUTES_H
