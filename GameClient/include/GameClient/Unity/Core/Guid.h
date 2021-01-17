//
// Created by Michal_Marszalek on 19.10.2020.
//

#ifndef RTS_GAME_GUID_H
#define RTS_GAME_GUID_H

#include <string>
#include <GameApi/HashCombine.h>
#include <nlohmann/json.hpp>

namespace Unity {
    using fileID = uint64_t;

    struct GUID {
        uint64_t first;
        uint64_t second;

        GUID() {
            first = 0;
            second = 0;
        }

        explicit GUID(std::string str) {
            *this = str;
        }

        explicit operator std::string() const;

        GUID &operator=(const std::string &str);

        constexpr bool operator==(const GUID &l) const = default;

        constexpr auto operator<=>(const GUID &) const = default;

        [[nodiscard]] bool empty() const {
            return first == 0 && second == 0;
        }

        static GUID NewGuid();
    };

    inline void to_json(nlohmann::json &j, const GUID &p) {
        j = p.operator std::string();
    }

    inline void from_json(const nlohmann::json &j, GUID &p) {
        std::string s;
        if (!j.is_null()) { j.get_to(s); }
        p = s;
    }
}

MAKE_HASHABLE(Unity::GUID, t.first, t.second);

#endif //RTS_GAME_GUID_H
