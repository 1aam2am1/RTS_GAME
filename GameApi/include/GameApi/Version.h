//
// Created by Michal_Marszalek on 06.08.2020.
//

#ifndef RTS_GAME_VERSION_H
#define RTS_GAME_VERSION_H

#include <string_view>

namespace GameApi {

    class Version {
    public:
        static const std::string_view GIT_DIRTY;
        static const std::string_view GIT_SHA;
        static const std::string_view GIT_TAG;
        static const std::string_view DATE;
    };
}

#endif //RTS_GAME_VERSION_H
