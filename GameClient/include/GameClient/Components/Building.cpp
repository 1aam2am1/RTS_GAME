//
// Created by Michal_Marszalek on 2021-01-24.
//

#include "Building.h"
#include "PrefabFunc.h"
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>

ADD_USER_COMPONENT(Building, type, building, parent, time)

void Building::FixedUpdate() {
    if (!parent) {
        Destroy(gameObject());
    }

    if (!building) {
        auto it = std::find_if(parent->ship_production.begin(), parent->ship_production.end(),
                               [&](auto &&s) { return s == type; });
        if (it != parent->ship_production.end()) {
            building = true;
            parent->ship_production.erase(it);
            t.restart();
        }
    } else {
        if (t.getElapsedTime().asSeconds() >= time) {
            building = false;

            auto p = transform()->localPosition();
            p.x += 1;
            Prefab_func::create_ship(parent, type, p);
        }
    }
}

void Building::Start() {
    if (!parent) {
        Destroy(gameObject());
    } else {
        parent->AddBuilding(type);
        gameObject()->layer = parent->gameObject()->layer;
    }
}

void Building::OnDestroy() {
    if (parent)
        parent->RemoveBuilding(type);
}
