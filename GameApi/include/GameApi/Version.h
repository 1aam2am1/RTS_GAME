//
// Created by Michal_Marszalek on 06.08.2020.
//

#ifndef RTS_GAME_VERSION_H
#define RTS_GAME_VERSION_H

#include <string>

namespace GameApi {

    class Version {
    public:
        static const std::string GIT_DIRTY;
        static const std::string GIT_SHA;
        static const std::string GIT_TAG;
        static const std::string DATE;
    };
}

#endif //RTS_GAME_VERSION_H
