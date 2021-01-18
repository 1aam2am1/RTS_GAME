//
// Created by Michal_Marszalek on 16.09.2020.
//

#ifndef RTS_GAME_SETTERGETTERP_H
#define RTS_GAME_SETTERGETTERP_H

#include <GameApi/SGBase.h>

template<typename T, typename C>
class SetterGetterP : public SGBase<T> {
public:
    SetterGetterP() = delete;

    constexpr SetterGetterP(C *parent, void (C::*set)(T), T (C::*get)()) noexcept: p(parent), s(set),
                                                                                   g(get) {}

    SetterGetterP(const SetterGetterP &) = delete;

    SetterGetterP &operator=(const SetterGetterP &r) {
        set(r);
        return *this;
    }

    SetterGetterP &operator=(SetterGetterP &&) = delete;

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
    SetterGetterP &operator=(Y &&t) {
        set(std::forward<Y>(t));
        return *this;
    }

    T get() const {
        return (p->*g)();
    }

    template<typename Y>
    void set(Y &&t) {
        (p->*s)(std::forward<Y>(t));
    }

private:
    C *p;

    void (C::*set)(T);

    T (C::*get)();
};


#endif //RTS_GAME_SetterGetterP_H
