//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_MONO_STATE_H
#define RTS_GAME_MONO_STATE_H

#include <nlohmann/json.hpp>

enum class mono_state {
    wait = 0,
    attack = 1,
    flee = 2,
    resource = 3
};

NLOHMANN_JSON_SERIALIZE_ENUM(mono_state,
                             {{ mono_state::wait, "wait" }, { mono_state::attack, "attack" }, { mono_state::flee, "flee" }, { mono_state::resource, "resource" }})

#endif //RTS_GAME_MONO_STATE_H
