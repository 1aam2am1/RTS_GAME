//
// Created by Michal_Marszalek on 2021-02-07.
//

#ifndef RTS_GAME_SIGNALSYNCHRONIZER_H
#define RTS_GAME_SIGNALSYNCHRONIZER_H


#include "FullSynchronizer.h"
#include <GameApi/Signal.h>
#include <nlohmann/json.hpp>

class SignalSynchronizer : public FullSynchronizer {
public:
    void IntUpdate() override;

    void Start() override;
};


#endif //RTS_GAME_SIGNALSYNCHRONIZER_H
