//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "SerializerBase.h"
#include <GameClient/TPtr.h>
#include <GameClient/MetaData.h>

TPtr<Object> SerializerBase::Deserialize(std::string_view str, const nlohmann::json &j) {
    return Deserialize(MetaData::get_type(str), j);
}

TPtr<Object> SerializerBase::Deserialize(std::type_index type, const nlohmann::json &j) {
    auto constructor = MetaData::get_name_constructor(type).second;

    TPtr<Object> result{nullptr};
    result = constructor->create();

    Deserialize(result, j);

    return result;
}

nlohmann::json SerializerBase::operator()(const nlohmann::json *j) {
    if (j) { return *j; }
    return {};
}

void SerializerBase::operator()(nlohmann::json *j, const nlohmann::json &o) {
    *j = o;
}
