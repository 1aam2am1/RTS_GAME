//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_BUILDING_H
#define RTS_GAME_BUILDING_H

#include <Core/MonoBehaviour.h>
#include <SFML/System/Clock.hpp>
#include "ShipType.h"
#include "Enemy.h"

class Building : public MonoBehaviour {
public:
    ShipType type;
    TPtr<Enemy> parent;
    float time = 10;

private:
    void FixedUpdate() override;

    void Start() override;

    void OnDestroy() override;

    bool building = false;
    sf::Clock t;
};


#endif //RTS_GAME_BUILDING_H
