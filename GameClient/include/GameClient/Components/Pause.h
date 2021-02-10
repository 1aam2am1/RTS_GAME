//
// Created by Michal_Marszalek on 2021-02-04.
//

#ifndef RTS_GAME_PAUSE_H
#define RTS_GAME_PAUSE_H

#include <Core/MonoBehaviour.h>

class Pause : public MonoBehaviour {
public:
    void Start() override;

    void OnGUI() override;
};


#endif //RTS_GAME_PAUSE_H
