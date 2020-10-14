//
// Created by Michal_Marszalek on 12.10.2020.
//

#ifndef RTS_GAME_EMITTER_H
#define RTS_GAME_EMITTER_H

#include <string>
#include "Document.h"

class Emitter {
public:
    const std::string &message() const;

    const char *c_str() const;

    std::size_t size() const;

    bool good() const;

    const std::string &GetLastError() const;

    Emitter &operator<<(const Node &);

    Emitter &operator<<(Document);

    void clear();

private:
    std::string str;
    std::string error;
    int indent = 0;

    std::map<void *, bool> writed;

    std::string escape(std::string_view);
};


#endif //RTS_GAME_EMITTER_H
