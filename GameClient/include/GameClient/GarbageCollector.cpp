//
// Created by Michal_Marszalek on 01.10.2020.
//

#include "GarbageCollector.h"


auto &getDeletedObjects() {
    static std::vector<std::pair<std::shared_ptr<Object>, std::function<void(std::shared_ptr<Object>)>>> list;
    return list;
}

void GarbageCollector::add(std::shared_ptr<Object> object, std::function<void(std::shared_ptr<Object>)> f) {
    getDeletedObjects().push_back({object, f});
}

void GarbageCollector::clear() {
    /*for(auto& object : getDeletedObjects()){
        object.first->OnDeleate();
    }*/
    for (auto &object : getDeletedObjects()) {
        if (object.second)object.second(object.first);
    }
    getDeletedObjects().clear();
}
