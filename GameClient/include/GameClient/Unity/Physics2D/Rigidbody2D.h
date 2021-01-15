//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_RIGIDBODY2D_H
#define RTS_GAME_RIGIDBODY2D_H


#include <Core/Component.h>
#include <box2d/box2d.h>
#include <GameClient/Unity/Physics2D/RigidbodyType2D.h>


class Rigidbody2D : public Component {
public:
    ~Rigidbody2D();

    /// The physical behaviour type of the Rigidbody2D.
    RigidbodyType2D bodyType;

    /// Mass of the Rigidbody.
    float mass = 1.f;

    /// The rigidBody rotational inertia.
    float inertia = 0.f;

protected:
    void Awake() override;

private:
    friend class Collider2D;

    b2Body *body = nullptr;
};


#endif //RTS_GAME_RIGIDBODY2D_H
