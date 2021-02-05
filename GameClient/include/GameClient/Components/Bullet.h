//
// Created by Michal_Marszalek on 2021-02-04.
//

#ifndef RTS_GAME_BULLET_H
#define RTS_GAME_BULLET_H

#include <Core/MonoBehaviour.h>

class Bullet : public MonoBehaviour {
    TPtr<Rigidbody2D> body;
public:
    float force = 10;

    void Start() override;

    void FixedUpdate() override;
};


#endif //RTS_GAME_BULLET_H
