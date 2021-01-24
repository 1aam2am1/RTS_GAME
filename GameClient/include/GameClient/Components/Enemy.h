//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_ENEMY_H
#define RTS_GAME_ENEMY_H

#include <Core/MonoBehaviour.h>
#include <Core/Sprite.h>
#include "mono_state.h"
#include "Resource.h"
#include "Base.h"
#include "Ship.h"
#include "ShipType.h"

class Enemy : public MonoBehaviour {
public:
    mono_state cell = mono_state::resource;
    std::map<ResourceType, float> needed = {{ResourceType::food,  0.5f},
                                            {ResourceType::metal, 0.5f},
                                            {ResourceType::water, 0.5f}};

    void AddShip(TPtr<Ship>);

    void RemoveShip(TPtr<Ship>);

    TPtr<Sprite> ship_sprites[2];
protected:
    friend class Building;

    std::vector<ShipType> ship_production;

    TPtr<Base> baze;
    std::vector<TPtr<Ship>> ships;

    TPtr<Enemy> other_enemy;

    void Update() override = 0;

    void ProduceShip(ShipType);

protected:
    void Start() override {
        baze = GetComponent<Base>();
        auto ob = FindObjectsOfType<Enemy>();
        for (auto &o : ob) {
            if (o.get() != this) {
                other_enemy = o;
                return;
            }
        }
    }
};

#endif //RTS_GAME_ENEMY_H
