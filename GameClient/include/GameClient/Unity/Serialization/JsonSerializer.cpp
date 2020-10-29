//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "JsonSerializer.h"
#include <GameApi/IsInstance.h>
#include <GameApi/function_traits.h>


nlohmann::json JsonSerializer::Serialize(const Object *object) {
    nlohmann::json result;

    auto name = get_name_constructor(typeid(*object)).first;
    if (name.empty()) {
        throw std::runtime_error(
                "Object don't have valid type reflection: " + GameApi::demangle(typeid(*object).name()));
    }

    auto reflection = get_reflections(object);

    auto &serialized = result[name.data()];
    for (auto &it: reflection) {
        for (auto &it2 : it->get(object)) {
            auto &check = serialized[it2.first.data()];
            if (!check.is_null()) {
                GameApi::log(ERR.fmt("Second serialization of member with the same name: %s, object type: %s",
                                     it2.first.data(),
                                     GameApi::demangle(typeid(*object).name()).data()));
            }
            check = std::visit([this](auto &&p) {
                if constexpr (is_instance_v<decltype(p), std::function>) {
                    auto value = p();
                    return this->operator()(&value);
                } else {
                    return this->operator()(p);
                }
            }, it2.second);
        }
    }

    return result;
}

TPtr<Object> JsonSerializer::Deserialize(std::type_index type, const nlohmann::json &serialized) {
    auto constructor = get_name_constructor(type).second;

    TPtr<Object> result{nullptr};
    result = constructor->create();

    if (serialized.is_object()) {
        auto reflection = get_reflections(result.get());
        for (auto &it: reflection) {
            for (auto &it2 : it->get(result.get())) {
                auto check = serialized.find(it2.first.data());
                if (check == serialized.end()) {
                    GameApi::log(WARN.fmt("Deserialization, value name %s not found, object type: %s",
                                          it2.first.data(),
                                          GameApi::demangle(type.name()).data()));
                } else {
                    std::visit([this, &c = check.value()](auto &&p) {
                        if constexpr (is_instance_v<decltype(p), std::function>) {
                            function_traits_arg_t<decltype(p), 0> value;
                            this->operator()(&value, c);
                            p(value);
                        } else {
                            return this->operator()(p, c);
                        }
                    }, it2.second);
                }
            }
        }
    }

    return result;
}

nlohmann::json JsonSerializer::operator()(const int64_t *i) {
    return nlohmann::json(*i);
}

nlohmann::json JsonSerializer::operator()(const double *d) {
    return nlohmann::json(*d);
}

nlohmann::json JsonSerializer::operator()(const std::string *str) {
    return nlohmann::json(*str);
}

nlohmann::json JsonSerializer::operator()(const TPtr<Object> *object) {
    if (*object) {
        return Serialize(object->get());
    } else {
        return nlohmann::json();
    }
}


void JsonSerializer::operator()(int64_t *i, const nlohmann::json &j) {
    if (j.is_number()) {
        *i = j.get<int64_t>();
    } else {
        *i = 0;
    }
}

void JsonSerializer::operator()(double *d, const nlohmann::json &j) {
    if (j.is_number()) {
        *d = j.get<double>();
    } else {
        *d = 0;
    }
}

void JsonSerializer::operator()(std::string *str, const nlohmann::json &j) {
    if (j.is_string()) {
        *str = j.get<std::string>();
    } else {
        str->clear();
    }
}

void JsonSerializer::operator()(TPtr<Object> *o, const nlohmann::json &j) {
    if (j.is_object() && !j.empty()) {
        *o = SerializerBase::Deserialize(std::string_view{j.begin().key().data()}, j.begin().value());
    } else {
        *o = nullptr;
    }
}
