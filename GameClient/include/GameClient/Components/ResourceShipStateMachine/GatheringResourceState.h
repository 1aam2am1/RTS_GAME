//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_GATHERINGRESOURCESTATE_H
#define RTS_GAME_GATHERINGRESOURCESTATE_H

#include <StateMachine.hpp>
#include "Event.h"

struct FleeState;
struct AttackState;
struct TransportState;
struct GoToState;

struct GatheringResourceState : Will<On<FixedUpdateEvent, Nothing>, On<ChangedBaseModeEvent, Nothing>> {
    ResourceShip *ship = nullptr;

    using Will::handle;

    template<class T>
    void onEnter(T &) {
        onEnter();
    }

    void onEnter();

    Maybe<TransitionTo<TransportState>, TransitionTo<GoToState>> handle(UpdateEvent &e);

    Maybe<TransitionTo<AttackState>, TransitionTo<FleeState>> handle(AttackedEvent &e);
};


#endif //RTS_GAME_GATHERINGRESOURCESTATE_H
