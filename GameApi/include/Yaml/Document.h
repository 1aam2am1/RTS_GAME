//
// Created by Michal_Marszalek on 12.10.2020.
//

#ifndef RTS_GAME_DOCUMENT_H
#define RTS_GAME_DOCUMENT_H

#include <vector>
#include <Yaml/Node/Node.h>
#include <map>

class Document {
public:
    enum Type {
        NOPE,
        JSON,
        YAML1_2,
    };

    Type type;

    std::vector<Node> nodes;

    std::map<std::string, std::string> tags;
};


#endif //RTS_GAME_DOCUMENT_H
