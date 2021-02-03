//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "ResourceShip.h"
#include <GameClient/Components/ResourceShipStateMachine/Event.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Components/Enemy.h>
#include <GameClient/Components/Life/Attack.h>

ADD_USER_COMPONENT(ResourceShip, state, time_of_flee, resource_area, speed_of_gathering, resources, capacity)
EXPORT_STATE_MACHINE(ResourceShip::ST)

void ResourceShip::OnUpdate() {
    UpdateEvent event{};
    state.handle(event);
}

void ResourceShip::OnFixedUpdate() {
    FixedUpdateEvent event{};
    state.handle(event);
}

void ResourceShip::UpdateObjective() {
    ChangedBaseModeEvent event{};
    state.handle(event);
}

void ResourceShip::OnCollisionEnter2D(const Collision2D &other) {
    if (other.gameObject->layer != gameObject()->layer) {
        auto a = other.gameObject->GetComponent<Attack>();
        if (a) {
            AttackedEvent event{};
            state.handle(event);
        }
    }
}
