//
// Created by Michal_Marszalek on 11.08.2020.
//

#include <GameApi/Sink/OSLogSink.h>
#include "GlobalLogSource.h"

namespace GameApi {
    GlobalLogSource::GlobalLogSource() {
        add(std::shared_ptr<LogSink>{new OSLogSink()});
    }

    GlobalLogSource &GlobalLogSource::instance() {
        static GlobalLogSource s;
        return s;
    }

    void GlobalLogSource::log(const GameApi::Message &msg) {
        std::lock_guard l(m);
        for (auto &e : list) {
            if (msg.lvl >= e->level) {
                e->outputLogMessage(msg);
            }
        }
    }

    void GlobalLogSource::add(const std::shared_ptr<LogSink> &e) {
        std::lock_guard l(m);
        list.push_back(e);
    }

    void GlobalLogSource::remove(const std::shared_ptr<LogSink> &e) {
        std::lock_guard l(m);
        list.remove(e);
    }


    void log(const GameApi::Message &m) {
        GlobalLogSource::instance().log(m);
    }

}
