//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "LogSink.hpp"

namespace GameApi {
    LogSink::LogSink() {
        this->setLogLevel(logLevel::dbg);
    }

    void LogSink::setLogLevel(GameApi::logLevel l) {
        level = l;
    }
}
