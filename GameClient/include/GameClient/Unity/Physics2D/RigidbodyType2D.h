//
// Created by Michal_Marszalek on 15.01.2021.
//

#ifndef RTS_GAME_RIGIDBODYTYPE2D_H
#define RTS_GAME_RIGIDBODYTYPE2D_H

#include <box2d/box2d.h>
#include <nlohmann/json.hpp>

enum class RigidbodyType2D {
    Dynamic = b2_dynamicBody,      ///< Sets the Rigidbody2D to have dynamic behaviour.
    Kinematic = b2_kinematicBody,    ///< Sets the Rigidbody2D to have kinematic behaviour.
    Static = b2_staticBody       ///< Sets the Rigidbody2D to have static behaviour.
};

NLOHMANN_JSON_SERIALIZE_ENUM(RigidbodyType2D,
                             {{ RigidbodyType2D::Dynamic, "Dynamic" }, { RigidbodyType2D::Kinematic, "Kinematic" }, { RigidbodyType2D::Static, "Static" }})

#endif //RTS_GAME_RIGIDBODYTYPE2D_H
