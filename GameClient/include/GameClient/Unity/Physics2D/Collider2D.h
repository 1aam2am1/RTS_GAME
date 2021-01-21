//
// Created by Michal_Marszalek on 18.09.2020.
//

#ifndef RTS_GAME_COLLIDER2D_H
#define RTS_GAME_COLLIDER2D_H


#include <GameClient/Unity/Core/Behaviour.h>
#include "Rigidbody2D.h"
#include <SFML/System/Vector2.hpp>

class b2Body;

class b2Fixture;

class Collider2D : public Behaviour {
public:
    Collider2D();

    /// The Rigidbody2D attached to the Collider2D.
    TPtr<Rigidbody2D> attachedRigidbody;

    /// The local offset of the collider geometry.
    SetterEmitterP<sf::Vector2f, Collider2D> offset;

    ~Collider2D() override;

protected:
    friend class Rigidbody2D;

    friend class GameLoop;

    // Apply fixture to body
    virtual void Apply() = 0;

    virtual void Destroy() = 0;

    void Awake() final;

    void Refresh(); //Recreate fixture if created

    void OnEnable() final;

    void OnDisable() final;

    void OnDestroy() final;

    b2Body *GetBody();

    void Update() override {};

    void RecalculateMass();
};


#endif //RTS_GAME_COLLIDER2D_H
