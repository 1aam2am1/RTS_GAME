//
// Created by Michal_Marszalek on 01.12.2020.
//

#ifndef RTS_GAME_GUIDFILEIDPACK_H
#define RTS_GAME_GUIDFILEIDPACK_H


#include <GameClient/Unity/Core/Guid.h>

struct GUIDFileIDPack {
    Unity::GUID guid{};
    Unity::fileID id{};

    constexpr bool operator==(const GUIDFileIDPack &) const = default;

    constexpr auto operator<=>(const GUIDFileIDPack &) const = default;
};

MAKE_HASHABLE(GUIDFileIDPack, t.guid, t.id)

inline void to_json(nlohmann::json &j, const GUIDFileIDPack &p) {
    j.clear();
    j["guid"] = p.guid;
    j["fileID"] = p.id;
}

inline void from_json(const nlohmann::json &j, GUIDFileIDPack &p) {
    j.at("guid").get_to(p.guid);
    j.at("fileID").get_to(p.id);
}

#endif //RTS_GAME_GUIDFILEIDPACK_H
