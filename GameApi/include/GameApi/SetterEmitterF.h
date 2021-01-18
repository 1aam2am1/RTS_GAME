//
// Created by Michal_Marszalek on 2021-01-17.
//

#ifndef RTS_GAME_SETTEREMITTERF_H
#define RTS_GAME_SETTEREMITTERF_H

#include <functional>
#include <utility>
#include <GameApi/SGBase.h>

template<typename T>
class SetterEmitterF : public SGBase<T> {
public:
    SetterEmitterF() = delete;

    template<typename ...Args>
    constexpr explicit SetterEmitterF(std::function<void()> emit, Args... args) noexcept: e(std::move(emit)),
                                                                                          t(std::forward<Args>(
                                                                                                  args)...) {}

    template<typename ...Args>
    constexpr explicit SetterEmitterF(std::function<void(T)> emit, Args... args) noexcept: e2(std::move(emit)),
                                                                                           t(std::forward<Args>(
                                                                                                   args)...) {}

    SetterEmitterF(const SetterEmitterF &) = delete;

    SetterEmitterF<T> &operator=(const SetterEmitterF<T> &r) {
        set(r);
        return *this;
    }

    SetterEmitterF<T> &operator=(SetterEmitterF<T> &&) = delete;

    operator T() const {
        return get();
    }

    T operator()() const {
        return get();
    }

    template<typename Y>
    void operator()(Y &&t) {
        set(t);
    }

    template<typename Y>
    SetterEmitterF<T> &operator=(Y &&t) {
        set(std::forward<Y>(t));
        return *this;
    }

    T get() const {
        return t;
    }

    template<typename Y>
    void set(Y &&t2) {
        t = std::forward<Y>(t2);
        if (e) { e(); }
        else if (e2) { e2(t); }
    }

private:
    const std::function<void()> e;
    const std::function<void(T)> e2;

    T t;
};

#endif //RTS_GAME_SETTEREMITTERF_H
