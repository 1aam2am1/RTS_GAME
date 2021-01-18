//
// Created by Michal_Marszalek on 16.09.2020.
//

#ifndef RTS_GAME_SETTERGETTERF_H
#define RTS_GAME_SETTERGETTERF_H

#include <functional>
#include <GameApi/SGBase.h>

//TODO: Remove SetterGetterF as it bloat code, think of something better __declspec(property)
template<typename T>
class SetterGetterF : public SGBase<T> {
public:
    SetterGetterF() = delete;

    constexpr SetterGetterF(const std::function<void(const T &)> &set, const std::function<T()> &get) noexcept: s(set),
                                                                                                                g(get) {}

    SetterGetterF(const SetterGetterF &) = delete;

    SetterGetterF<T> &operator=(const SetterGetterF<T> &r) {
        set(r);
        return *this;
    }

    SetterGetterF<T> &operator=(SetterGetterF<T> &&) = delete;

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
    SetterGetterF<T> &operator=(Y &&t) {
        set(std::forward<Y>(t));
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
    const std::function<void(const T &)> s;
    const std::function<T()> g;
};


#endif //RTS_GAME_SETTERGETTERF_H
