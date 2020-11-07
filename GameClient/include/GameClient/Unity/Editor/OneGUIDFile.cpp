//
// Created by Michal_Marszalek on 06.11.2020.
//

#include "OneGuidFile.h"

OneGUIDFile &OneGUIDFile::operator=(const OneGUIDFile &r) {
    if (&r == this) { return *this; }

    this->path = r.path;
    this->importer = r.importer;
    this->dependency = r.dependency;
    this->main.reset(r.main);

    auto copy = r.object;
    for (auto iterator = this->object.begin(); iterator != this->object.end();) {
        auto it = copy.find(iterator->first);
        if (it != copy.end()) {
            iterator->second.reset(it->second);
            copy.erase(it);

            ++iterator;
        } else {
            iterator = object.erase(iterator);
        }
    }

    for (auto &it: copy) {
        this->object.emplace(it.first, it.second);
    }

    return *this;
}
