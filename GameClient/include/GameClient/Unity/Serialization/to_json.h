//
// Created by Michal_Marszalek on 12.01.2021.
//

#ifndef RTS_GAME_TO_JSON_H
#define RTS_GAME_TO_JSON_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

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

    template<typename T>
    inline void to_json(nlohmann::json &j, const Rect <T> &r) {
        j.clear();

        j["x"] = r.left;
        j["y"] = r.top;
        j["width"] = r.width;
        j["height"] = r.height;
    }

    template<typename T>
    inline void from_json(const nlohmann::json &j, Rect <T> &r) {
        r.left = j.at("x").get<T>();
        r.top = j.at("y").get<T>();
        r.width = j.at("width").get<T>();
        r.height = j.at("height").get<T>();
    }
}

#endif //RTS_GAME_TO_JSON_H
