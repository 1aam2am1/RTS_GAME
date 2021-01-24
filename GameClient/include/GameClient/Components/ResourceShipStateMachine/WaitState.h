//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_WAITSTATE_H
#define RTS_GAME_WAITSTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct AttackState;
struct FleeState;
struct GatheringResourceState;

struct WaitState : Will<On<FixedUpdateEvent, Nothing>, On<ChangedBaseModeEvent, Nothing>> {
    ResourceShip *ship = nullptr;

    using Will::handle;

    template<class T>
    void onEnter(T &) {
        onEnter();
    }

    void onEnter();

    OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>> handle(AttackedEvent &e);

    OneOf<TransitionTo<AttackState>, TransitionTo<FleeState>, TransitionTo<GatheringResourceState>>
    handle(UpdateEvent &);
};


#endif //RTS_GAME_WAITSTATE_H
