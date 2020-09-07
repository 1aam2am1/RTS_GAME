//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_GLOBALLOGSOURCE_H
#define RTS_GAME_GLOBALLOGSOURCE_H

#include <list>
#include <memory>
#include <mutex>
#include "LogSource.h"
#include "LogSink.hpp"
#include "MessageCreator.h"

namespace GameApi {
    class GlobalLogSource final : public LogSource {
        GlobalLogSource();

    public:
        static GlobalLogSource &instance();

        void log(const Message &m) override;

        void add(const std::shared_ptr<LogSink> &);

        void remove(const std::shared_ptr<LogSink> &);

    private:
        std::mutex m;
        std::list<std::shared_ptr<LogSink>> list;
    };

    void log(const Message &m);
}

#endif //RTS_GAME_GLOBALLOGSOURCE_H
