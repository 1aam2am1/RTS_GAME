//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "Message.h"
#include "ThreadId.h"
#include "BasicString.h"
#include <chrono>
#include <iomanip>


namespace GameApi {

    Message::Message() : line{}, threadID(getCurrentThreadID()), timeStamp(
            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())) {

    }

    Message::operator std::string() const {
        return std::string{}
               + "\x1b[" + lvl->colorCode + "m" + to_width_string(lvl->name, 4)
               + "\x1b[m " + filename + ":" + to_string(line) + " "
               + "(" + functionName
               + ", thread " + to_string(threadID) + ")"
               + ": " + text;
    }

    std::ostream &operator<<(std::ostream &os, const Message &msg) {
        os << std::string{msg};

        return os;
    }
}
