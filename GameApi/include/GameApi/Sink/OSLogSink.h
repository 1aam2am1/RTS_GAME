//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_OSLOGSINK_H
#define RTS_GAME_OSLOGSINK_H

#include <GameApi/LogSink.hpp>

namespace GameApi {

    class OSLogSink : public LogSink {
    public:
        OSLogSink();

        void outputLogMessage(const Message &message) override;
    };

}

#endif //RTS_GAME_OSLOGSINK_H
