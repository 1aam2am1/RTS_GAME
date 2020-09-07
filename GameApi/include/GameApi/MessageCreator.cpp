//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "MessageCreator.h"

#include <utility>


GameApi::MessageCreator::MessageCreator(std::string filename, uint32_t line, std::string functionName,
                                        GameApi::logLevel lvl) : StringFormatter<MessageCreator>(msg.text) {
    msg.filename = std::move(filename);
    msg.line = line;
    msg.functionName = std::move(functionName);
    msg.lvl = lvl;
}
