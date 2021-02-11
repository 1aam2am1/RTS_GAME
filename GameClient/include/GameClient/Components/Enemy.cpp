//
// Created by Michal_Marszalek on 2021-01-22.
//

#include "Enemy.h"
#include "Placer.h"
#include "PrefabFunc.h"
#include "UserControls.h"
#include "AttackShip.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>

ADD_USER_COMPONENT(Enemy, attack_sprite, resource_sprite, resources, cell, needed, ship_production, targets_AShip,
                   targets_RShip, targets_ABuild, targets_RBuild)

void Enemy::AddShip(TPtr<Ship> s) {
    auto it = std::find_if(ships.begin(), ships.end(), [&](auto &&t) { return t == s; });
    if (it == ships.end()) {
        ships.emplace_back(s);
        s->parent = static_pointer_cast<Enemy>(shared_from_this());
        if (dynamic_cast<AttackShip *>(s.get())) {
            targets_AShip.emplace_back(s->transform());
        } else {
            targets_RShip.emplace_back(s->transform());
        }
    }
}

void Enemy::AddBuilding(TPtr<Building> b) {
    auto it = std::find_if(buildings.begin(), buildings.end(), [&](auto &&t) { return t == b; });
    if (it == buildings.end()) {
        if (b->gameObject() != gameObject()) { buildings.emplace_back(b); }
        b->parent = static_pointer_cast<Enemy>(shared_from_this());

        if (b->type == ShipType::Attack) {
            targets_ABuild.emplace_back(b->transform());
        } else {
            targets_RBuild.emplace_back(b->transform());
        }
    }
}

void Enemy::RemoveBuilding(TPtr<Building> b) {
    auto it = std::find_if(buildings.begin(), buildings.end(), [&](auto &&t) { return t == b; });
    if (it != buildings.end()) {
        buildings.erase(it);
        b->parent = nullptr;
    }
    if (b->gameObject() == gameObject()) {
        b->parent = nullptr;
    }

    std::erase_if(targets_ABuild, [](auto &p) { return p.expired(); });
    std::erase_if(targets_RBuild, [](auto &p) { return p.expired(); });
}

void Enemy::RemoveShip(TPtr<Ship> s) {
    auto it = std::find_if(ships.begin(), ships.end(), [&](auto &&t) { return t == s; });
    if (it != ships.end()) {
        ships.erase(it);
        s->parent = nullptr;
    }

    std::erase_if(targets_AShip, [](auto &p) { return p.expired(); });
    std::erase_if(targets_RShip, [](auto &p) { return p.expired(); });
}

void Enemy::ProduceShip(ShipType type) {
    if (resources[ResourceType::food] >= 100) {
        switch (type) {
            case ShipType::Resource:
                if (resources[ResourceType::metal] >= 100) {
                    resources[ResourceType::food] -= 100;
                    resources[ResourceType::metal] -= 100;
                    ship_production.emplace_back(type);
                }
                break;
            case ShipType::Attack:
                if (resources[ResourceType::metal] >= 200) {
                    resources[ResourceType::food] -= 100;
                    resources[ResourceType::metal] -= 200;
                    ship_production.emplace_back(type);
                }
        }
    }
}

void Enemy::ChangedObjective() {
    if (old_cell != cell()) {
        for (auto &&s : ships) {
            if (s) { s->UpdateObjective(); }
        }
    }
}

TPtr<GameObject> Enemy::ProduceBuilding(ShipType type) {
    if (resources[ResourceType::food] >= 500 && resources[ResourceType::water] >= 1000) {
        switch (type) {
            case ShipType::Resource:
                if (resources[ResourceType::metal] >= 300) {
                    resources[ResourceType::food] -= 500;
                    resources[ResourceType::metal] -= 300;
                    resources[ResourceType::water] -= 1000;
                    return Prefab_func::create_building(shared_from_this(), type);
                }
                break;
            case ShipType::Attack:
                if (resources[ResourceType::metal] >= 600) {
                    resources[ResourceType::food] -= 500;
                    resources[ResourceType::metal] -= 600;
                    resources[ResourceType::water] -= 1000;
                    return Prefab_func::create_building(shared_from_this(), type);
                }
        }
    }

    return {};
}

void Enemy::Update() {

}
