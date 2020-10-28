//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "SerializerBase.h"

std::vector<std::shared_ptr<ObjectFactory::Reflect>> SerializerBase::get_reflections(const Object *o) {
    std::vector<std::shared_ptr<ObjectFactory::Reflect>> result;

    for (auto &it: ObjectFactory::reflection) {
        if (it.second.second->check(o)) {
            result.emplace_back(it.second.second);
        }
    }

    return result;
}

decltype(ObjectFactory::reflection)::mapped_type SerializerBase::get_name_constructor(std::type_index type) {
    auto it = ObjectFactory::reflection.find(type);

    if (it != ObjectFactory::reflection.end()) {
        return it->second;
    }

    return {};
}

std::type_index SerializerBase::get_type(std::string_view str) {
    auto it = ObjectFactory::name_type.find(str);

    if (it != ObjectFactory::name_type.end()) {
        return it->second;
    }
    std::string s;
    s.reserve(str.length() + 5 + 55 + 1);
    s = "Type ";
    s += str;
    s += " don't have mapped object constructor: use EXPORT_CLASS";
    throw std::runtime_error(s);
}

TPtr<Object> SerializerBase::Deserialize(std::string_view str, const nlohmann::json &j) {
    return Deserialize(get_type(str), j);
}
