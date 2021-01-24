//
// Created by Michal_Marszalek on 2021-01-23.
//

#ifndef RTS_GAME_MAYBE_H
#define RTS_GAME_MAYBE_H

#include "OneOf.h"
#include "Nothing.h"

template<typename... Action>
struct Maybe : public OneOf<Action..., Nothing> {
    using OneOf<Action..., Nothing>::OneOf;
};


#endif //RTS_GAME_MAYBE_H
