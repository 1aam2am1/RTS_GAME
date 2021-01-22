//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_ENEMY_H
#define RTS_GAME_ENEMY_H

#include <Core/MonoBehaviour.h>
#include "mono_state.h"
#include "Resource.h"
#include "Base.h"
#include "ShipAi.h"

class Enemy : public MonoBehaviour {
public:
    mono_state cell = mono_state::resource;
    std::map<ResourceType, float> needed;

private:
    TPtr<Base> baze;
    std::vector<TPtr<ShipAi>> ships;

    TPtr<Enemy> other_enemy;

    void Start() {
        baze = GetComponent<Base>();
        auto ob = FindObjectsOfType<Enemy>();
        for (auto &o : ob) {
            if (o.get() != this) {
                other_enemy = o;
                return;
            }
        }
    }

    void Update() override;
};

#endif //RTS_GAME_ENEMY_H
