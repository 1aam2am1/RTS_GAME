//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_SHIPAI_H
#define RTS_GAME_SHIPAI_H

#include <Core/MonoBehaviour.h>
#include <numbers>
#include "Resource.h"
#include "mono_state.h"

class Enemy;

class ShipAi : public MonoBehaviour {
public:
    TPtr<Transform> target;
    TPtr<Rigidbody2D> rigidbody;
    TPtr<Enemy> parent;

    mono_state state = mono_state::resource;

    float force = 10;
    float visibility = 7;

    void Awake() override {};

    void Start() override {
        rigidbody = GetComponent<Rigidbody2D>();
        rigidbody->drag = 1;
    }

    void GetTarget();

    void Update() override {
        if (!target) {
            GetTarget();
        }

        if (!parent) {
            Destroy(transform());
        }
    };

    void FixedUpdate() override {
        if (target) {
            auto where = transform()->localPosition() - target->localPosition();

            transform()->localRotation = std::atan2(where.y, where.x) * 180.0f / std::numbers::pi + 90.0f;

            auto length = (std::sqrt(where.x * where.x + where.y * where.y));
            where /= length;
            if (length > 5.0f) {
                where *= force;
            } else {
                where *= force * length / 5.0f;
            }


            rigidbody->AddForce({-where.x, -where.y});
        }

    }

    void FindNext();

    void OnDestroy() override {

    }
};


#endif //RTS_GAME_SHIPAI_H
