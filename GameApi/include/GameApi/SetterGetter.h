//
// Created by Michal_Marszalek on 16.09.2020.
//

#ifndef RTS_GAME_SETTERGETTER_H
#define RTS_GAME_SETTERGETTER_H

#include <functional>

template<typename T>
class SetterGetter {
public:
    SetterGetter(const std::function<void(const T &)> &set, const std::function<const T &()> &get) : set(set),
                                                                                                     get(get) {}

    SetterGetter(const SetterGetter &) = delete;

    operator const T &() const {
        return get();
    }

    SetterGetter<T> &operator=(const T &t) {
        set(t);
        return *this;
    }

private:
    std::function<void(const T &)> set;
    std::function<const T &()> get;
};


#endif //RTS_GAME_SETTERGETTER_H
