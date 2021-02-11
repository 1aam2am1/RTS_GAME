//
// Created by Michal_Marszalek on 2021-02-02.
//

#ifndef RTS_GAME_LIFE_H
#define RTS_GAME_LIFE_H


#include <Core/MonoBehaviour.h>
#include <GameClient/Components/Network/SignalSynchronizer.h>

class Life : public MonoBehaviour {
public:
    float life = 10;

    void attacked(float i, const TPtr<GameObject> &enemy);

    void Start() override;

private:
    TPtr<Synchronizer> signal;

    std::vector<GUIDFileIDPack> attacker;
};


#endif //RTS_GAME_LIFE_H
