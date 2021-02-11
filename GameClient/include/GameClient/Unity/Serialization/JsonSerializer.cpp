//
// Created by Michal_Marszalek on 28.10.2020.
//

#include "JsonSerializer.h"
#include <GameClient/MetaData.h>
#include <GameClient/Unity/Macro.h>
#include "ISerializationCallbackReceiver.h"


nlohmann::json JsonSerializer::Serialize(const Object *object) {
    return InternalSerialize(object, true);
}

nlohmann::json JsonSerializer::InternalSerialize(const Object *object, bool force) {
    if (object == nullptr) { return {}; }

    GUIDFileIDPack id;

    nlohmann::json result;

    auto ob = object->shared_from_this();
    bool serialize = false;

    auto it = serialize_map.find(ob);
    if (it != serialize_map.end()) {
        id = it->second;
    } else {
        auto[id2, s2] = serialize_node_callback(ob);
        serialize_map[ob] = id2;
        serialize = s2;

        id = id2;
    }

    if (serialize || force) {
        result = TrueSerialize(object);
        result["__Node_id"] = id;

        return result;
    } else {
        return id;
    }
}

TPtr<> JsonSerializer::Deserialize(const nlohmann::json &j) {
    auto lock = register_check();

    if (j.size() == 2 && j.contains("guid") && j.contains("fileID")) [[unlikely]] {
        auto pack = j.get<GUIDFileIDPack>();
        auto it = deserialize_map.find(pack);
        if (it != deserialize_map.end()) {
            return it->second;
        }

        auto ob = deserialize_get_node_callback(pack);
        if (ob) {
            deserialize_map[pack] = ob;
        }
        return ob;
    }

    if (!j.is_object() || j.empty()) { return {}; }
    for (auto &ob : j.items()) {
        if (ob.key().starts_with("__")) { continue; }

        auto result = MetaData::getReflection(ob.key()).CreateInstance();

        Update(result, ob.value());

        /** Check some internal values **/
        {
            if (j.contains("__Node_id")) {
                try {
                    auto pack = j.at("__Node_id").get<GUIDFileIDPack>();
                    deserialize_map[pack] = result;
                }
                EXCEPTION_PRINT
            }
        }

        return result;
    }

    return {};
}

nlohmann::json JsonSerializer::TrueSerialize(const Object *object) {
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
            return this->operator()(p.second());
        }, it.second);
    }

    return result;
}

void JsonSerializer::Update(TPtr<Object> result, const nlohmann::json &serialized) {
    auto lock = register_check();

    /// Thing this through, do we want to clear data, that could not be serialized?
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
                    if (p.first) {
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

std::shared_ptr<int> JsonSerializer::register_check() {
    std::shared_ptr<int> lock = checking.lock();
    if (!lock) [[unlikely]] {
        lock = std::shared_ptr<int>((int *) 1, [&](auto) {
            for (auto &it : to_check) {
                auto ob = deserialize_map.find(it.first);
                if (ob != deserialize_map.end()) {
                    it.second(ob->second);
                    continue;
                }

                auto ob2 = deserialize_get_node_callback(it.first);
                deserialize_map[it.first] = ob2;

                it.second(ob2);
            }

            to_check.clear();
        });
        checking = lock;
    }

    return lock;
}

std::pair<GUIDFileIDPack, bool> JsonSerializer::serialize_node_callback(TPtr<const Object>) {
    static uint64_t max_id;
    return {{Unity::GUID("0"), ++max_id}, true};
}

TPtr<> JsonSerializer::deserialize_get_node_callback(GUIDFileIDPack) {
    return TPtr<>();
}

nlohmann::json JsonSerializer::operator()(TPtr<> o) {
    return InternalSerialize(o.get(), false);
}

void JsonSerializer::operator()(const std::function<void(TPtr<Object>)> &o, const nlohmann::json &j) {
    if (j.size() == 2 && j.contains("guid") && j.contains("fileID")) {
        auto pack = j.get<GUIDFileIDPack>();

        to_check.emplace(pack, o);
        return;
    }

    o(Deserialize(j));
}
