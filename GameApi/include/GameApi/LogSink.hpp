//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_LOGSINK_HPP
#define RTS_GAME_LOGSINK_HPP

#include "Message.h"

namespace GameApi {
    class GlobalLogSource;

    class LogSink {
    public:
        LogSink();

        virtual ~LogSink() = default;

        virtual void outputLogMessage(const Message &) = 0;

        void setLogLevel(logLevel l);

    private:
        logLevel level;

        friend class GlobalLogSource;
    };
}

#endif //RTS_GAME_LOGSINK_HPP
