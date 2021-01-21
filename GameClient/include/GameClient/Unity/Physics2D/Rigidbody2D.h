//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_RIGIDBODY2D_H
#define RTS_GAME_RIGIDBODY2D_H


#include <Core/Component.h>
#include <GameClient/Unity/Physics2D/RigidbodyType2D.h>
#include <SFML/System/Vector2.hpp>

class b2Body;

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

    /// Coefficient of angular drag.
    SetterEmitterP<float, Rigidbody2D> angularDrag;

    /// Coefficient of drag.
    SetterEmitterP<float, Rigidbody2D> drag;

    enum ForceMode2D {
        Force, ///<	Add a force to the Rigidbody2D, using its mass.
        Impulse, ///<	Add an instant force impulse to the rigidbody2D, using its mass.
    };

    void AddForce(sf::Vector2f force, ForceMode2D mode = Force);

protected:
    void Awake() override;

private:
    friend class Collider2D;

    void RecalculateMass();

    b2Body *body = nullptr;

    void UnityOnActiveChange(bool) override;

    void ChangeBody();
};


#endif //RTS_GAME_RIGIDBODY2D_H
