//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "SerializerBase.h"
#include <GameClient/TPtr.h>
#include <GameClient/MetaData.h>

TPtr<Object> SerializerBase::Deserialize(std::string_view str, const nlohmann::json &j) {
    return Deserialize(MetaData::getReflection(str).type, j);
}

TPtr<Object> SerializerBase::Deserialize(std::type_index type, const nlohmann::json &j) {
    auto reflection = MetaData::getReflection(type);

    TPtr<Object> result;
    result = reflection.CreateInstance();

    Deserialize(result, j);

    return result;
}

nlohmann::json SerializerBase::operator()(const nlohmann::json &j) {
    return j;
}

void SerializerBase::operator()(const std::function<void(nlohmann::json)> &j, const nlohmann::json &o) {
    j(o);
}

TPtr<> SerializerBase::Deserialize(const nlohmann::json &j) {
    if (!j.is_object() || j.empty()) { return {}; }
    for (auto &ob : j.items()) {
        if (ob.key().starts_with("__")) { continue; }

        return Deserialize(ob.key(), ob.value());
    }

    return {};
}
