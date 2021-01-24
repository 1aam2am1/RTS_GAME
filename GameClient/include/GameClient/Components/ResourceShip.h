//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_RESOURCESHIP_H
#define RTS_GAME_RESOURCESHIP_H

#include <GameClient/Components/ResourceShipStateMachine/AttackState.h>
#include <GameClient/Components/ResourceShipStateMachine/WaitState.h>
#include <GameClient/Components/ResourceShipStateMachine/TransportState.h>
#include <GameClient/Components/ResourceShipStateMachine/FleeState.h>
#include <GameClient/Components/ResourceShipStateMachine/GatheringResourceState.h>
#include <GameClient/Components/ResourceShipStateMachine/GoToState.h>
#include <GameClient/Unity/StateMachine/StateMachine.h>
#include "Ship.h"
#include "Resource.h"

class ResourceShip : public Ship {
public:
    using ST = StateMachine<AttackState, WaitState, TransportState, FleeState, GatheringResourceState, GoToState>;
    float time_of_flee = 10; ///How long to flee
    float resource_area = 1;
    float speed_of_gathering = 10; //How fast we gather resources
    std::map<ResourceType, float> resources;
    float capacity = 50;

protected:
    void OnUpdate() override;

    void OnFixedUpdate() override;

private:
    ST state{AttackState{{}, this}, WaitState{{}, this}, TransportState{{}, this}, FleeState{{}, this},
             GatheringResourceState{{}, this}, GoToState{{}, this}};
};


#endif //RTS_GAME_RESOURCESHIP_H
