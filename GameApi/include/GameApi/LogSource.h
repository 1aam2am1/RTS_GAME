//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_LOGSOURCE_H
#define RTS_GAME_LOGSOURCE_H

#include "Message.h"

namespace GameApi {

    class LogSource {
    public:
        virtual ~LogSource() = default;

        virtual void log(const Message &m) = 0;
    };
}

#endif //RTS_GAME_LOGSOURCE_H
