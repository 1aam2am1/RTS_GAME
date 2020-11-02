//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "MetaData.h"

decltype(MetaData::reflection) MetaData::reflection;
decltype(MetaData::name_type) MetaData::name_type;
decltype(MetaData::ext_importer) MetaData::ext_importer;
decltype(MetaData::ext_priority) MetaData::ext_priority;


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

std::pair<const std::type_index, int64_t> MetaData::get_importer(std::string_view str) {
    auto it = ext_importer.find(str);

    if (it == ext_importer.end()) {
        std::string s;
        s.reserve(str.length() + 5 + 55 + 1);
        s = "Type ";
        s += str;
        s += " don't have mapped object importer: use EXPORT_IMPORTER";
        throw std::runtime_error(s);
    }

    auto it2 = ext_priority.find(it->second);
    int64_t priority;

    if (it2 == ext_priority.end()) {
        GameApi::log(WARN.fmt("Importer: %s, with class %s using default priority. 0",
                              str.data(),
                              GameApi::demangle(it->second.name()).data()));
        priority = 0;
    } else {
        priority = it2->second;
    }

    return std::make_pair(it->second, priority);
}
