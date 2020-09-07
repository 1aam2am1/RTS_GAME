#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by Michal_Marszalek on 11.08.2020.
//

#ifndef RTS_GAME_MESSAGECREATOR_H
#define RTS_GAME_MESSAGECREATOR_H

#include "StringFormatter.h"
#include "Message.h"
#include "GameAPI/Config.h"
#include "BasicString.h"

namespace GameApi {

    class MessageCreator : public StringFormatter<MessageCreator> {
    public:
        MessageCreator(std::string filename, uint32_t line, std::string functionName, logLevel lvl);

        inline operator const Message &() const {
            return this->msg;
        }

        inline operator Message &() {
            return this->msg;
        }

    private:
        Message msg;

    };

#if defined(__GNUC__)
#define FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC_NAME __FUNCSIG__
#else
#define FUNC_NAME __FUNCTION__
#endif

#define MSG_LVL(LVL) \
    ::GameApi::MessageCreator( \
        ::GameApi::removePrefix(__FILE__, PROJECT_SOURCE_DIR), \
        __LINE__, \
        FUNC_NAME, \
        LVL)


#define MSG(LVL) MSG_LVL(GameApi::logLevel:: LVL)

#define SPAM MSG(spam)
#define DBG MSG(dbg)
#define INFO MSG(info)
#define WARN MSG(warn)
#define ERR MSG(err)
#define CRIT MSG(crit)

}

#endif //RTS_GAME_MESSAGECREATOR_H

#pragma clang diagnostic pop
