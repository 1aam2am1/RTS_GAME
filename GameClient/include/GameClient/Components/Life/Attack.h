//
// Created by Michal_Marszalek on 2021-02-02.
//

#ifndef RTS_GAME_ATTACK_H
#define RTS_GAME_ATTACK_H


#include <Core/MonoBehaviour.h>

class Attack : public MonoBehaviour {
public:
    float attack = 1;

protected:
public:
    void OnCollisionEnter2D(const Collision2D &other) override;
};


#endif //RTS_GAME_ATTACK_H
