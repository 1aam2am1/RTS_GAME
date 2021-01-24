//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_STATEMACHINE_H
#define RTS_GAME_STATEMACHINE_H

#include <variant>
#include <tuple>
#include <utility>

template<typename... States>
class StateMachine {
public:
    StateMachine() = default;

    explicit StateMachine(States... states) : states(std::move(states)...) {}

    template<typename Event>
    void handle(Event &event) {
        auto passEventToState = [this, &event](auto statePtr) {
            auto action = statePtr->handle(event);
            action.execute(*this, *statePtr, event);
        };
        std::visit(passEventToState, currentState);
    }

    template<typename State>
    State &transitionTo() {
        auto &state = std::get<State>(states);
        currentState = &state;
        enter(state);

        return state;
    }

    template<typename T>
    void visit(T &&t) const {
        std::visit(std::forward<T>(t), currentState);
    }

private:
    std::tuple<States...> states{};
    std::variant<States *...> currentState{&std::get<0>(states)};

    void enter(...) {
    }

    template<typename State>
    auto enter(State &state) -> decltype(state.onEnter()) {
        return state.onEnter();
    }
};


#endif //RTS_GAME_STATEMACHINE_H
