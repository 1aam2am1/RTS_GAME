//
// Created by Michal_Marszalek on 2021-02-12.
//

#ifndef RTS_GAME_SHIPDUMMY_H
#define RTS_GAME_SHIPDUMMY_H


#include <Core/MonoBehaviour.h>

class Enemy;

class ShipDummy : public MonoBehaviour {
public:
    TPtr<Enemy> parent;
};


#endif //RTS_GAME_SHIPDUMMY_H
