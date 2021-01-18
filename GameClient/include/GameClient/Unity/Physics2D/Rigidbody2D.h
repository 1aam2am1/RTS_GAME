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
    Rigidbody2D();

    ~Rigidbody2D();

    /// The physical behaviour type of the Rigidbody2D.
    SetterEmitterP<RigidbodyType2D, Rigidbody2D> bodyType;

    /// Mass of the Rigidbody.
    SetterEmitterP<float, Rigidbody2D> mass;

    /// The rigidBody rotational inertia.
    SetterEmitterP<float, Rigidbody2D> inertia;

protected:
    void Awake() override;

private:
    friend class Collider2D;

    b2Body *body = nullptr;

    void UnityOnActiveChange(bool) override;

    void ChangeBody();
};


#endif //RTS_GAME_RIGIDBODY2D_H
