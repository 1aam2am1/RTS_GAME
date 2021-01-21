//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_BOXCOLLIDER2D_H
#define RTS_GAME_BOXCOLLIDER2D_H


#include "Collider2D.h"

class BoxCollider2D : public Collider2D {
public:

    BoxCollider2D();

    /// The width and height of the rectangle.
    /// These values are specified relative to a center point, so the distance from the center to the left edge is actually width/2.
    SetterEmitterP<sf::Vector2f, BoxCollider2D> size;

protected:
    void Apply() override;

    void Destroy() override;

    void ChangeSize();

    b2Fixture *fixture = nullptr;
};


#endif //RTS_GAME_BOXCOLLIDER2D_H
