//
// Created by Michal_Marszalek on 2021-02-12.
//

#ifndef RTS_GAME_WINLOSE_H
#define RTS_GAME_WINLOSE_H


#include <Core/MonoBehaviour.h>
#include <GameClient/Components/Network/SignalSynchronizer.h>
#include "Enemy.h"

class WinLose : public MonoBehaviour {
public:
    void Start() override;

    void Update() override;

public:
    TPtr<SignalSynchronizer> signal;
    std::vector<TPtr<Enemy>> enemy;
};


#endif //RTS_GAME_WINLOSE_H
