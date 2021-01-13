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
                           const std::function<bool(Object *, Object *)> &copyInstance,
                           const MetaData::flags_type &f) :
        name(n), type(t), CreateInstance(createInstance), CopyInstance(copyInstance), flags(f) {}

MetaData::ReflectFull::ReflectFull(const std::string_view name, const std::type_index type,
                                   const std::function<TPtr<Object>()> &createInstance,
                                   const std::function<bool(Object *, Object *)> &copyInstance,
                                   const MetaData::flags_type &flag,
                                   const std::vector<std::pair<std::string_view, TU>> &f,
                                   const std::map<std::string_view, std::type_index> &t) :
        Reflect(name, type, createInstance, copyInstance, flag), getFields(f), getTPtrType(t) {}

MetaData::ReflectFull MetaData::getReflection(Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;
    std::map<std::string_view, std::type_index> types;

    for (auto it: reflection) {
        if (it.second.check(ob)) {
            for (auto mem: it.second.members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
            for (auto mem: it.second.TPtr_type) {
                types.emplace(mem.first, mem.second);
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, object_reflection.flags, fields, types};

    return result;
}

MetaData::ReflectFull MetaData::getReflection(const Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;
    std::map<std::string_view, std::type_index> types;

    for (auto it : reflection) {
        if (it.second.check(ob)) {
            for (auto mem: it.second.c_members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
            for (auto mem: it.second.TPtr_type) {
                types.emplace(mem.first, mem.second);
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, object_reflection.flags, fields, types};

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

static const MetaData::flags_type empty_flag_reflection{};

MetaData::Reflect MetaData::getReflection(std::type_index type) {
    std::string_view name;
    std::function<TPtr<Object>()> constructor;
    std::function<bool(Object *, Object *)> copy;
    const MetaData::flags_type *flags = &empty_flag_reflection;

    auto object_reflection = reflection.find(type);
    if (object_reflection != reflection.end()) {
        name = object_reflection->second.name;
        if (object_reflection->second.create) {
            constructor = object_reflection->second.create;
        } else {
            constructor = []() -> TPtr<> {
                GameApi::log(ERR.fmt("Tried creating object with empty constructor function"));
                return {};
            };
        }

        copy = object_reflection->second.copy;
        flags = &object_reflection->second.flags;
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

    MetaData::Reflect result{name, type, constructor, copy, *flags};

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
