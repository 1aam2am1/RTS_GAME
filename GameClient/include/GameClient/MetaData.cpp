//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "MetaData.h"

decltype(MetaData::reflection) MetaData::reflection;
decltype(MetaData::name_type) MetaData::name_type;


std::vector<std::shared_ptr<MetaData::Reflect>> MetaData::get_reflections(const Object *o) {
    std::vector<std::shared_ptr<Reflect>> result;

    for (auto &it: reflection) {
        if (it.second.second->check(o)) {
            result.emplace_back(it.second.second);
        }
    }

    return result;
}

std::pair<std::string_view, const std::shared_ptr<MetaData::Reflect>>
MetaData::get_name_constructor(std::type_index type) {
    auto it = reflection.find(type);

    if (it != reflection.end()) {
        return it->second;
    }

    return {};
}

std::type_index MetaData::get_type(std::string_view str) {
    auto it = name_type.find(str);

    if (it != name_type.end()) {
        return it->second;
    }
    std::string s;
    s.reserve(str.length() + 5 + 55 + 1);
    s = "Type ";
    s += str;
    s += " don't have mapped object constructor: use EXPORT_CLASS";
    throw std::runtime_error(s);
}
