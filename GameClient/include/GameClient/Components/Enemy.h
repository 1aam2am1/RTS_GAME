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

    std::vector<TPtr<Ship>> ships;
    std::vector<TPtr<Building>> buildings;

    TPtr<Enemy> other_enemy;

    void Update() override;

    void ProduceShip(ShipType);

    TPtr<GameObject> ProduceBuilding(ShipType);

    void ChangedObjective();

protected:
    void Start() override {
        auto ob = FindObjectsOfType<Enemy>();
        for (auto &o : ob) {
            if (o.get() != this) {
                other_enemy = o;
                return;
            }
        }
        signal = GetComponent<SignalSynchronizer>();
        if (signal) {
            auto t = static_pointer_cast<Enemy>(shared_from_this());
            signal->OnMessage.connect([t](uint32_t i, auto) {
                if (!t) { return; }
                if (i == 10) {
                    t->other_enemy = nullptr;
                    t->Update();
                }
            });
        }
    }

    mono_state old_cell = mono_state::resource;

private:
    TPtr<SignalSynchronizer> signal;
};

#endif //RTS_GAME_ENEMY_H
