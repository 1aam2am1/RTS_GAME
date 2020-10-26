//
// Created by Michal_Marszalek on 19.10.2020.
//

#ifndef RTS_GAME_GUID_H
#define RTS_GAME_GUID_H

#include <string>
#include <GameApi/HashCombine.h>

namespace Unity {
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

        GUID &operator=(std::string str);

        bool operator==(const GUID &l) const {
            return first == l.first && second == l.second;
        }

        bool operator!=(const GUID &l) const {
            return !(*this == l);
        }

        bool operator<(const GUID &l) const {
            return first < l.first ? true : (second < l.second ? true : false);
        }

        bool operator>(const GUID &l) const {
            return first > l.first ? true : (second > l.second ? true : false);
        }

        bool empty() const {
            return first == 0 && second == 0;
        }

        static GUID NewGuid();
    };

}

MAKE_HASHABLE(Unity::GUID, t.first, t.second);

#endif //RTS_GAME_GUID_H
