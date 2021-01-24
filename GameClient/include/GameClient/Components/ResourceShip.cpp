//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "ResourceShip.h"
#include <GameClient/Components/ResourceShipStateMachine/Event.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Components/Enemy.h>

ADD_USER_COMPONENT(ResourceShip, state, time_of_flee, resource_area, speed_of_gathering, resources)
EXPORT_STATE_MACHINE(ResourceShip::ST)

void ResourceShip::OnUpdate() {
    UpdateEvent event{};
    state.handle(event);
}

void ResourceShip::OnFixedUpdate() {
    FixedUpdateEvent event{};
    state.handle(event);
}
