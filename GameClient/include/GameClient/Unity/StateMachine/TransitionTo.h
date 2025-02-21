//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_TRANSITIONTO_H
#define RTS_GAME_TRANSITIONTO_H


template<typename TargetState>
class TransitionTo {
public:
    template<typename Machine, typename State, typename Event>
    void execute(Machine &machine, State &prevState, const Event &event) {
        leave(prevState, event);
        TargetState &newState = machine.template transitionTo<TargetState>();
        enter(newState, event);
    }

private:
    void leave(...) {
    }

    template<typename State, typename Event>
    auto leave(State &state, const Event &event) -> decltype(state.onLeave(event)) {
        return state.onLeave(event);
    }

    void enter(...) {
    }

    template<typename State, typename Event>
    auto enter(State &state, const Event &event) -> decltype(state.onEnter(event)) {
        return state.onEnter(event);
    }
};


#endif //RTS_GAME_TRANSITIONTO_H
