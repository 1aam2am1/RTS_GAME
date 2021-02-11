//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_ENEMY_H
#define RTS_GAME_ENEMY_H

#include <Core/MonoBehaviour.h>
#include <Core/Sprite.h>
#include "mono_state.h"
#include "Resource.h"
#include "Ship.h"
#include "ShipType.h"
#include "Building.h"


class Enemy : public MonoBehaviour {
public:
    SetterEmitterP<mono_state, Enemy> cell = {this, &Enemy::ChangedObjective, mono_state::resource};
    std::map<ResourceType, float> needed = {{ResourceType::food,  0.5f},
                                            {ResourceType::metal, 0.5f},
                                            {ResourceType::water, 0.5f}};

    std::map<ResourceType, float> resources = {{ResourceType::food,  100},
                                               {ResourceType::metal, 200},
                                               {ResourceType::water, 100}};

    void AddShip(TPtr<Ship>);

    void RemoveShip(TPtr<Ship>);

    void AddBuilding(TPtr<Building>);

    void RemoveBuilding(TPtr<Building>);

    TPtr<Sprite> attack_sprite;
    TPtr<Sprite> resource_sprite;

    std::vector<ShipType> ship_production;

    std::vector<TPtr<Transform>> targets_AShip;
    std::vector<TPtr<Transform>> targets_RShip;
    std::vector<TPtr<Transform>> targets_ABuild;
    std::vector<TPtr<Transform>> targets_RBuild;

    TPtr<Enemy> other_enemy;

    void Update() override;

    void ProduceShip(ShipType);

    TPtr<GameObject> ProduceBuilding(ShipType);

    void ChangedObjective();

    std::vector<TPtr<Ship>> ships;
    std::vector<TPtr<Building>> buildings;

protected:
    void Start() override {
        auto ob = FindObjectsOfType<Enemy>();
        for (auto &o : ob) {
            if (o.get() != this) {
                other_enemy = o;
                return;
            }
        }
    }

    mono_state old_cell = mono_state::resource;
};

#endif //RTS_GAME_ENEMY_H
