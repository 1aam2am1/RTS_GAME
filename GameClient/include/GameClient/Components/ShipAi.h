//
// Created by Michal_Marszalek on 2021-01-21.
//

#ifndef RTS_GAME_SHIPAI_H
#define RTS_GAME_SHIPAI_H

#include <Core/MonoBehaviour.h>
#include <numbers>
#include "Resource.h"

class ShipAi : public MonoBehaviour {
public:
    TPtr<Resource> target;
    TPtr<Rigidbody2D> rigidbody;

    float force = 10;

    void Awake() override {};

    void Start() override {
        rigidbody = GetComponent<Rigidbody2D>();
        rigidbody->drag = 1;
    }

    void Update() override {

    };

    void FixedUpdate() override {
        if (target) {
            auto where = transform()->localPosition() - target->transform()->localPosition();

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
