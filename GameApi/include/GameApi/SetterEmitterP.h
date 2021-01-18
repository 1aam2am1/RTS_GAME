//
// Created by Michal_Marszalek on 2021-01-17.
//

#ifndef RTS_GAME_SETTEREMITTERP_H
#define RTS_GAME_SETTEREMITTERP_H

#include <GameApi/SGBase.h>

template<typename T, typename C>
class SetterEmitterP : public SGBase<T> {
public:
    SetterEmitterP() = delete;

    template<typename ...Args>
    constexpr SetterEmitterP(C *parent, void (C::*emit)(), Args... args) noexcept: p(parent), e(emit),
                                                                                   t(std::forward<Args>(args)...) {}

    template<typename ...Args>
    constexpr SetterEmitterP(C *parent, void (C::*emit)(T), Args... args) noexcept: p(parent), e2(emit),
                                                                                    t(std::forward<Args>(args)...) {}

    SetterEmitterP(const SetterEmitterP &) = delete;

    SetterEmitterP &operator=(const SetterEmitterP &r) {
        set(r);
        return *this;
    }

    SetterEmitterP &operator=(SetterEmitterP &&) = delete;

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
    SetterEmitterP &operator=(Y &&t) {
        set(std::forward<Y>(t));
        return *this;
    }

    T get() const {
        return t;
    }

    template<typename Y>
    void set(Y &&t2) {
        t = std::forward<Y>(t2);
        if (e) { (p->*e)(); }
        else if (e2) { (p->*e2)(t); }
    }

private:
    friend C;

    C *p;

    void (C::*e)() = nullptr;

    void (C::*e2)(T) = nullptr;

    T t;
};

#endif //RTS_GAME_SETTEREMITTERP_H
