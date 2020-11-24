//
// Created by Michal_Marszalek on 16.09.2020.
//

#ifndef RTS_GAME_SETTERGETTER_H
#define RTS_GAME_SETTERGETTER_H

#include <functional>

template<typename T>
class SetterGetter {
public:
    SetterGetter() = delete;

    SetterGetter(const std::function<void(const T &)> &set, const std::function<T()> &get) : s(set),
                                                                                             g(get) {}

    SetterGetter(const SetterGetter &) = delete;

    SetterGetter<T> &operator=(const SetterGetter<T> &r) {
        set(r);
        return *this;
    }

    SetterGetter<T> &operator=(SetterGetter<T> &&) = delete;

    operator T() const {
        return g();
    }

    T operator()() const {
        return g();
    }

    template<typename Y>
    SetterGetter<T> &operator=(Y &&t) {
        s(std::forward<Y>(t));
        return *this;
    }

    T get() const {
        return g();
    }

    template<typename Y>
    void set(Y &&t) {
        s(std::forward<Y>(t));
    }

private:
    std::function<void(const T &)> s;
    std::function<T()> g;
};


#endif //RTS_GAME_SETTERGETTER_H
