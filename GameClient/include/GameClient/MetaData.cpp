//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "MetaData.h"

decltype(MetaData::reflection) MetaData::reflection;
decltype(MetaData::name_type) MetaData::name_type;
decltype(Importers::ext_importer) Importers::ext_importer;
decltype(Importers::ext_priority) Importers::ext_priority;


MetaData::Reflect::Reflect(const std::string_view n, const std::type_index t,
                           const std::function<TPtr<Object>()> &createInstance,
                           const std::function<bool(Object *, Object *)> &copyInstance) :
        name(n), type(t), CreateInstance(createInstance), CopyInstance(copyInstance) {}

MetaData::ReflectFull::ReflectFull(const std::string_view name, const std::type_index type,
                                   const std::function<TPtr<Object>()> &createInstance,
                                   const std::function<bool(Object *, Object *)> &copyInstance,
                                   const std::vector<std::pair<std::string_view, TU>> &f) :
        Reflect(name, type, createInstance, copyInstance), getFields(f) {}

MetaData::ReflectFull MetaData::getReflection(Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;

    for (auto it : reflection) {
        if (it.second.check(ob)) {
            for (auto mem: it.second.members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, fields};

    return result;
}

MetaData::ReflectFull MetaData::getReflection(const Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;

    for (auto it : reflection) {
        if (it.second.check(ob)) {
            for (auto mem: it.second.c_members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, fields};

    return result;
}

MetaData::Reflect MetaData::getReflection(std::string_view str) {
    auto it = name_type.find(str);

    if (it != name_type.end()) {
        return getReflection(it->second);
    }
    std::string s;
    s.reserve(str.length() + 5 + 55 + 1);
    s = "Type ";
    s += str;
    s += " don't have mapped object constructor: use EXPORT_CLASS";
    throw std::runtime_error(s);
}

MetaData::Reflect MetaData::getReflection(std::type_index type) {
    std::string_view name;
    std::function<TPtr<Object>()> constructor;
    std::function<bool(Object *, Object *)> copy;

    auto object_reflection = reflection.find(type);
    if (object_reflection != reflection.end()) {
        name = object_reflection->second.name;
        constructor = object_reflection->second.create;
        copy = object_reflection->second.copy;
    } else {
        constructor = [type]() -> TPtr<Object> {
            std::string s;
            std::string name = GameApi::demangle(type.name());
            s.reserve(name.length() + 5 + 55 + 1);
            s = "Type ";
            s += name;
            s += " don't have mapped object constructor: use EXPORT_CLASS";
            throw std::runtime_error(s);
        };

        copy = [type](Object *, Object *) -> bool {
            std::string s;
            std::string name = GameApi::demangle(type.name());
            s.reserve(name.length() + 5 + 50 + 1);
            s = "Type ";
            s += name;
            s += " don't have mapped copy function: use EXPORT_CLASS";
            throw std::runtime_error(s);
        };
    }

    MetaData::Reflect result{name, type, constructor, copy};

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////

std::pair<const std::type_index, int64_t> Importers::get_importer(std::string_view str) {
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

bool Importers::exists_importer(std::string_view str) {
    auto it = ext_importer.find(str);

    if (it != ext_importer.end()) {
        return MetaData::getReflection(it->second).CreateInstance.operator bool();
    }

    return false;
}
