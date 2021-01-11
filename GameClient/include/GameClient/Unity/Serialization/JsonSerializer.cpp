//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "JsonSerializer.h"
#include <GameApi/IsInstance.h>
#include <GameClient/MetaData.h>
#include <GameClient/TPtr.h>
#include "ISerializationCallbackReceiver.h"


nlohmann::json JsonSerializer::Serialize(const Object *object) {
    nlohmann::json result;

    {
        auto d = dynamic_cast<ISerializationCallbackReceiver *>(const_cast<Object *>(object));
        if (d) { d->OnBeforeSerialize(); }
    }
    auto reflection = MetaData::getReflection(object);
    if (reflection.name.empty()) {
        GameApi::log(
                ERR.fmt("Object don't have valid type reflection:  %s",
                        GameApi::demangle(typeid(*object).name()).data()));
        return {};
    }

    auto &serialized = result[reflection.name.data()];
    for (auto &it: reflection.getFields) {
        auto &check = serialized[it.first.data()];
        if (!check.is_null()) {
            GameApi::log(ERR.fmt("Second serialization of member with the same name: %s, object type: %s",
                                 it.first.data(),
                                 GameApi::demangle(typeid(*object).name()).data()));
        }
        check = std::visit([this](auto &&p) {
            if (!p.second) {
                return nlohmann::json{};
            }
            if constexpr (is_instance_v<decltype(p.second()), std::vector>) {
                if constexpr(is_instance_v<typename decltype(p.second())::value_type, TPtr>) {
                    return this->operator()(p.second());
                } else {
                    auto vec = p.second();
                    nlohmann::json result;

                    for (auto &it: vec) {
                        result.emplace_back(this->operator()(it));
                    }

                    return result;
                }
            } else {
                return this->operator()(p.second());
            }
        }, it.second);
    }

    return result;
}

void JsonSerializer::Deserialize(TPtr<Object> result, const nlohmann::json &serialized) {

    if (serialized.is_object()) {
        auto reflection = MetaData::getReflection(result.get());
        for (auto &it: reflection.getFields) {
            auto check = serialized.find(it.first.data());
            if (check == serialized.end()) {
                GameApi::log(WARN.fmt("Deserialization, value name %s not found, object type: %s",
                                      it.first.data(),
                                      GameApi::demangle(
                                              typeid(*result.get()).name()).data()));
            } else {
                std::visit([this, &c = check.value()](auto &&p) {
                    if constexpr (is_instance_v<function_traits_arg_t<decltype(p.first), 0>, std::vector>) {
                        if (p.first) {
                            using vector_type = function_traits_arg_t<decltype(p.first), 0>;
                            using object_type = typename vector_type::value_type;

                            if constexpr (std::is_same_v<object_type, TPtr<Object>>) {
                                this->operator()(p.first, c);
                            } else {
                                auto f = p.first;
                                std::shared_ptr<vector_type> value(new vector_type,
                                                                   [f](auto ptr) {
                                                                       f(*ptr);
                                                                       delete ptr;
                                                                   });
                                if (c.is_array()) {
                                    value->resize(c.size());
                                    int i = 0;
                                    for (auto &v: c) {
                                        std::function<void(object_type)> func = [value, i](auto n) {
                                            (*value)[i] = n;
                                        };
                                        this->operator()(func, v);
                                        ++i;
                                    }
                                }
                            }
                        }
                    } else if (p.first) {
                        this->operator()(p.first, c);
                    }
                }, it.second);
            }
        }
    }

    {
        auto d = dynamic_cast<ISerializationCallbackReceiver *>(result.get());
        if (d) { d->OnAfterDeserialize(); }
    }
}

nlohmann::json JsonSerializer::operator()(int64_t i) {
    return nlohmann::json(i);
}

nlohmann::json JsonSerializer::operator()(double d) {
    return nlohmann::json(d);
}

nlohmann::json JsonSerializer::operator()(const std::string &str) {
    return nlohmann::json(str);
}

nlohmann::json JsonSerializer::operator()(bool b) {
    return nlohmann::json(b);
}

nlohmann::json JsonSerializer::operator()(sf::Color c) {
    nlohmann::json result;

    result["red"] = c.r;
    result["green"] = c.g;
    result["blue"] = c.b;
    result["alpha"] = c.a;

    return result;
}

nlohmann::json JsonSerializer::operator()(const TPtr<Object> &object) {
    if (object) {
        return Serialize(object.get());
    } else {
        return nlohmann::json();
    }
}

nlohmann::json JsonSerializer::operator()(const std::vector<TPtr<Object>> &vec) {
    nlohmann::json result;

    for (auto &it: vec) {
        result.emplace_back(this->operator()(it));
    }

    return result;
}


void JsonSerializer::operator()(const std::function<void(int64_t)> &i, const nlohmann::json &j) {
    if (j.is_number()) {
        i(j.get<int64_t>());
    } else {
        i(0);
    }
}

void JsonSerializer::operator()(const std::function<void(double)> &d, const nlohmann::json &j) {
    if (j.is_number()) {
        d(j.get<double>());
    } else {
        d(0);
    }
}

void JsonSerializer::operator()(const std::function<void(std::string)> &str, const nlohmann::json &j) {
    if (j.is_string()) {
        str(j.get<std::string>());
    } else {
        str("");
    }
}

void JsonSerializer::operator()(const std::function<void(bool)> &b, const nlohmann::json &j) {
    if (j.is_number()) {
        b(j.get<bool>());
    } else {
        b(false);
    }
}

void JsonSerializer::operator()(const std::function<void(sf::Color)> &c, const nlohmann::json &j) {
    if (j.is_object() && !j.empty()) {
        sf::Color col;

        col.r = j["red"].get<int>();
        col.g = j["green"].get<int>();
        col.b = j["blue"].get<int>();
        col.a = j["alpha"].get<int>();

        c(col);
    } else {
        c({});
    }
}

void JsonSerializer::operator()(const std::function<void(TPtr<Object>)> &o, const nlohmann::json &j) {
    if (j.is_object() && !j.empty()) {
        o(SerializerBase::Deserialize(j));
    } else {
        o(TPtr<>{});
    }
}

void JsonSerializer::operator()(const std::function<void(std::vector<TPtr<Object>>)> &f, const nlohmann::json &j) {
    std::shared_ptr<std::vector<TPtr<Object>>> value(new std::vector<TPtr<Object>>,
                                                     [f](auto ptr) {
                                                         f(*ptr);
                                                         delete ptr;
                                                     });

    if (j.is_array()) {
        value->resize(j.size());
        int i = 0;
        for (auto &v: j) {
            auto func = [value, i](TPtr<Object> n) {
                (*value)[i] = n;
            };
            this->operator()(func, v);
            ++i;
        }
    }
}
