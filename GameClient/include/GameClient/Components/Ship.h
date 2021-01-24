//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_SHIP_H
#define RTS_GAME_SHIP_H

#include <Core/MonoBehaviour.h>

class Enemy;

class Ship : public MonoBehaviour {
public:
    TPtr<Transform> target;
    sf::Vector3f target_position_goto{};

    float force = 10;
    float visibility = 7;
    float attack_force = 1;
    float life = 10;

    TPtr<Enemy> parent;
    TPtr<Rigidbody2D> rigidbody;
protected:
    virtual void OnUpdate() = 0;

    virtual void OnFixedUpdate() = 0;

    void OnCollisionStay2D(const Collision2D &other) override;

private:
    void Start() final;

    void Update() final;

    void FixedUpdate() final;

    void OnDestroy() final;
};


#endif //RTS_GAME_SHIP_H
