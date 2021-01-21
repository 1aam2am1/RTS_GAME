//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_CIRCLECOLLIDER2D_H
#define RTS_GAME_CIRCLECOLLIDER2D_H


#include "Collider2D.h"

class CircleCollider2D : public Collider2D {
public:
    CircleCollider2D();

    SetterEmitterP<float, CircleCollider2D> radius; ///<    Radius of the circle.

private:
    b2Fixture *fixture = nullptr;
protected:
    void Apply() override;

    void Destroy() override;


};


#endif //RTS_GAME_CIRCLECOLLIDER2D_H
