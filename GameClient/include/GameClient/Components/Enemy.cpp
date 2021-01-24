//
// Created by Michal_Marszalek on 2021-01-22.
//

#include "Enemy.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>

EXPORT_CLASS(Enemy, cell, needed, ships, ship_production)

void Enemy::AddShip(TPtr<Ship> s) {
    auto it = std::find_if(ships.begin(), ships.end(), [&](auto &&t) { return t == s; });
    if (it == ships.end()) {
        ships.emplace_back(s);
        s->parent = static_pointer_cast<Enemy>(shared_from_this());
    }
}

void Enemy::RemoveShip(TPtr<Ship> s) {
    auto it = std::find_if(ships.begin(), ships.end(), [&](auto &&t) { return t == s; });
    if (it != ships.end()) {
        ships.erase(it);
        s->parent = nullptr;
    }
}

void Enemy::ProduceShip(ShipType type) {
    if (baze->resources[ResourceType::food] >= 100) {
        switch (type) {
            case ShipType::Resource:
                if (baze->resources[ResourceType::metal] >= 100) {
                    baze->resources[ResourceType::food] -= 100;
                    baze->resources[ResourceType::metal] -= 100;
                    ship_production.emplace_back(type);
                }
                break;
            case ShipType::Attack:
                if (baze->resources[ResourceType::metal] >= 200) {
                    baze->resources[ResourceType::food] -= 100;
                    baze->resources[ResourceType::metal] -= 200;
                    ship_production.emplace_back(type);
                }
        }
    }
}

void Enemy::createBuildingPlacer(ShipType) {

}


