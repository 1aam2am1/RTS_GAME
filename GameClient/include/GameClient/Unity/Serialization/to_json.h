//
// Created by Michal_Marszalek on 12.01.2021.
//

#ifndef RTS_GAME_TO_JSON_H
#define RTS_GAME_TO_JSON_H

#include <SFML/Graphics/Color.hpp>

namespace sf {
    inline void to_json(nlohmann::json &j, const Color &c) {
        j.clear();

        j["red"] = c.r;
        j["green"] = c.g;
        j["blue"] = c.b;
        j["alpha"] = c.a;
    }

    inline void from_json(const nlohmann::json &j, Color &c) {
        c.r = j.at("red").get<int>();
        c.g = j.at("green").get<int>();
        c.b = j.at("blue").get<int>();
        c.a = j.at("alpha").get<int>();
    }
}

#endif //RTS_GAME_TO_JSON_H
