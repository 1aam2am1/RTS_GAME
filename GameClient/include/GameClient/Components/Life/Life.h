//
// Created by Michal_Marszalek on 2021-02-02.
//

#ifndef RTS_GAME_LIFE_H
#define RTS_GAME_LIFE_H


#include <Core/MonoBehaviour.h>

class Life : public MonoBehaviour {
public:
    float life = 10;

    void attacked(float i);
};


#endif //RTS_GAME_LIFE_H
