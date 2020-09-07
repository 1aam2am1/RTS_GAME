//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_MESSAGE_H
#define RTS_GAME_MESSAGE_H

#include <string>
#include <cstdint>
#include <chrono>
#include "LogLevel.h"

namespace GameApi {

    struct Message {

        Message();

        std::string text;

        std::string filename;

        uint32_t line;

        std::string functionName;

        logLevel lvl;

        const uint32_t threadID;

        const std::chrono::nanoseconds timeStamp;

        explicit operator std::string() const;
    };

    std::ostream &operator<<(std::ostream &os, const Message &msg);
}


#endif //RTS_GAME_MESSAGE_H
