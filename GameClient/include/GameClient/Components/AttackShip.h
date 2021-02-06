//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_ATTACKSHIP_H
#define RTS_GAME_ATTACKSHIP_H

#include <GameClient/Components/AttackShipStateMachine/AttackState.h>
#include <GameClient/Components/AttackShipStateMachine/WaitState.h>
#include <GameClient/Components/AttackShipStateMachine/Patrol.h>
#include <GameClient/Components/AttackShipStateMachine/FleeState.h>
#include <GameClient/Components/AttackShipStateMachine/GoToState.h>
#include "Ship.h"

class AttackShip : public Ship {
public:
    using ST = StateMachine<AttackMachine::WaitState, AttackMachine::Patrol, AttackMachine::AttackState, AttackMachine::FleeState, AttackMachine::GoToState>;
    float time_of_flee = 2; ///How long to flee

protected:
    void OnUpdate() override;

    void OnFixedUpdate() override;

public:
    void UpdateObjective() override;

    void OnCollisionEnter2D(const Collision2D &other) override;

private:
    ST state{AttackMachine::WaitState{{},
                                      {this}}, AttackMachine::Patrol{{},
                                                                     {this}}, AttackMachine::AttackState{{}, this},
             AttackMachine::FleeState{{}, this}, AttackMachine::GoToState{{},
                                                                          {this}}};
};


#endif //RTS_GAME_ATTACKSHIP_H
