//
// Created by Michal_Marszalek on 18.09.2020.
//

#ifndef RTS_GAME_COROUTINE_H
#define RTS_GAME_COROUTINE_H


#include "YieldInstruction.h"

class Coroutine : public YieldInstruction {
public:
    YieldReturn operator()() override { return {}; }

private:
    int handle;
};


#endif //RTS_GAME_COROUTINE_H
