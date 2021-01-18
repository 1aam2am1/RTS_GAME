//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "MetaData.h"

#include <utility>

decltype(MetaData::reflection) MetaData::reflection;
decltype(MetaData::name_type) MetaData::name_type;
decltype(Importers::ext_importer) Importers::ext_importer;
decltype(Importers::ext_priority) Importers::ext_priority;
decltype(Enums::reflection) Enums::reflection;


MetaData::Reflect::Reflect(std::string_view n, std::type_index t,
                           std::function<TPtr<Object>()> createInstance,
                           std::function<bool(Object *, Object *)> copyInstance,
                           std::function<bool(Object *)> checkInstance,
                           const MetaData::flags_type &f) :
        name(n), type(t), CreateInstance(std::move(createInstance)), CopyInstance(std::move(copyInstance)),
        CheckInstance(std::move(checkInstance)), flags(f) {}

MetaData::ReflectFull::ReflectFull(std::string_view name, std::type_index type,
                                   std::function<TPtr<Object>()> createInstance,
                                   std::function<bool(Object *, Object *)> copyInstance,
                                   std::function<bool(Object *)> checkInstance,
                                   const MetaData::flags_type &flag,
                                   std::vector<std::pair<std::string_view, TU>> f,
                                   std::map<std::string_view, std::type_index> t) :
        Reflect(name, type, std::move(createInstance), std::move(copyInstance), std::move(checkInstance), flag),
        getFields(std::move(f)), getTPtrType(std::move(t)) {}

MetaData::ReflectFull MetaData::getReflection(Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;
    std::map<std::string_view, std::type_index> types;

    for (auto &&it: reflection) {
        if (it.second.check(ob)) {
            for (auto &&mem: it.second.members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
            for (auto &&mem: it.second.TPtr_type) {
                types.emplace(mem.first, mem.second);
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, object_reflection.CheckInstance,
                                 object_reflection.flags, fields, types};

    return result;
}

MetaData::ReflectFull MetaData::getReflection(const Object *ob) {
    auto object_reflection = getReflection(typeid(*ob));

    std::vector<std::pair<std::string_view, MetaData::TU>> fields;
    std::map<std::string_view, std::type_index> types;

    for (auto &&it : reflection) {
        if (it.second.check(ob)) {
            for (auto &&mem: it.second.c_members) {
                fields.emplace_back(mem.first, mem.second(ob));
            }
            for (auto &&mem: it.second.TPtr_type) {
                types.emplace(mem.first, mem.second);
            }
        }
    }

    MetaData::ReflectFull result{object_reflection.name, typeid(*ob), object_reflection.CreateInstance,
                                 object_reflection.CopyInstance, object_reflection.CheckInstance,
                                 object_reflection.flags, fields, types};

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
    std::function<bool(Object *)> check;
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
        check = object_reflection->second.check;
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

        check = [type](Object *o) { return typeid(o) == type; };
    }

    MetaData::Reflect result{name, type, constructor, copy, check, *flags};

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

/////////////////////////////////////////////////////////////////////////////////////

Enums::Reflect Enums::getReflection(std::type_index type) {
    auto it = reflection.find(type);
    if (it != reflection.end()) {
        return Reflect{it->second};
    }
    static const Data empty{};

    return Reflect{empty};
}

std::string_view Enums::Reflect::to_value(uint64_t i) {
    auto it = std::find_if(d.members.begin(), d.members.end(), [&](auto &&p) { return p.first == i; });
    if (it != d.members.end()) {
        return it->second;
    }
    return "";
}

uint64_t Enums::Reflect::to_value(std::string_view name) {
    auto it = std::find_if(d.members.begin(), d.members.end(), [&](auto &&p) { return p.second == name; });
    if (it != d.members.end()) {
        return it->first;
    }
    return 0;
}
