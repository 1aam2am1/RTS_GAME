//
// Created by Michal_Marszalek on 2021-02-05.
//

#ifndef RTS_GAME_USERCONTROLS_H
#define RTS_GAME_USERCONTROLS_H

#include <Core/MonoBehaviour.h>
#include "Enemy.h"
#include "Building.h"


class UserControls : public MonoBehaviour {
public:

    float upper = 110.f;
    TPtr<Enemy> base;
    TPtr<Transform> camera;

    void Start() override;

    void OnGUI() override;

    void createBuildingPlacer(ShipType type);
};


#endif //RTS_GAME_USERCONTROLS_H
