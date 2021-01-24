//
// Created by Michal_Marszalek on 2021-01-22.
//

#ifndef RTS_GAME_SHIPTYPE_H
#define RTS_GAME_SHIPTYPE_H

#include <nlohmann/json.hpp>

enum class ShipType {
    Resource = 0,
    Attack = 1,
};

NLOHMANN_JSON_SERIALIZE_ENUM(ShipType,
                             {{ ShipType::Resource, "Resource" }, { ShipType::Attack, "Attack" }})


#endif //RTS_GAME_SHIPTYPE_H
