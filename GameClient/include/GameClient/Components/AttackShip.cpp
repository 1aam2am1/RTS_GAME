//
// Created by Michal_Marszalek on 2021-01-23.
//

#include "AttackShip.h"
#include <GameClient/Components/ResourceShipStateMachine/Event.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Components/Enemy.h>
#include <GameClient/Components/Life/Attack.h>
#include <Core/Attributes.h>

ADD_USER_COMPONENT(AttackShip, state, time_of_flee)
EXPORT_STATE_MACHINE(AttackShip::ST)
ADD_ATTRIBUTE(AttackShip, DontNetworkSynchronize)

void AttackShip::OnUpdate() {
    AttackMachine::UpdateEvent event{};
    state.handle(event);
}

void AttackShip::OnFixedUpdate() {
    AttackMachine::FixedUpdateEvent event{};
    state.handle(event);
}

void AttackShip::UpdateObjective() {
    AttackMachine::ChangedBaseModeEvent event{};
    state.handle(event);
}

void AttackShip::OnCollisionEnter2D(const Collision2D &other) {
    if (other.gameObject->layer != gameObject()->layer) {
        auto a = other.gameObject->GetComponent<Attack>();
        if (a) {
            AttackMachine::AttackedEvent event{};
            state.handle(event);
        }
    }
}
