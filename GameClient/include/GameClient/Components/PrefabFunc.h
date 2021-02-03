//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_PREFABFUNC_H
#define RTS_GAME_PREFABFUNC_H


#include <Core/GameObject.h>
#include "ShipType.h"

class Enemy;

namespace Prefab_func {
    TPtr<GameObject> create_ship(TPtr<Enemy> parent, ShipType type, sf::Vector3f position);

    TPtr<GameObject> create_building(TPtr<Enemy> parent, ShipType type);

    TPtr<GameObject> create_bullet(TPtr<GameObject> parent, sf::Vector3f position, float direction);
};


#endif //RTS_GAME_PREFABFUNC_H
