//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_JSONSERIALIZER_H
#define RTS_GAME_JSONSERIALIZER_H

#include <GameClient/Unity/Core/Object.h>
#include <GameClient/GuidFileIdPack.h>
#include <GameClient/TPtr.h>
#include "to_json.h"
#include <nlohmann/json.hpp>
#include <typeindex>
#include <cstdint>
#include <string>
#include <Macro.h>


class JsonSerializer {
public:
    /// json -> {{'TYPE NAME': Serialization}, OPTIONAL{'__NODE': Node}}
    nlohmann::json Serialize(const Object *);

    /// json -> {{'TYPE NAME': Serialization}, OPTIONAL{'__NODE': Node}}
    TPtr<> Deserialize(const nlohmann::json &);

    /// json -> {value: Serialization from Serialize}
    /// \param o Object to serialize, not null!
    /// \param j Values of object
    virtual void Update(TPtr<Object> o, const nlohmann::json &);

    const std::unordered_map<TPtr<const Object>, GUIDFileIDPack> &get_id_map() { return serialize_map; }

protected:
    /// Must give unique GUIDFileIDPack, if returns true object is serialized as it is not saved elsewhere
    virtual std::pair<GUIDFileIDPack, bool> serialize_node_callback(TPtr<const Object>);

    /// Gives valid object or nullptr
    virtual TPtr<> deserialize_get_node_callback(GUIDFileIDPack);

    //region Serialize
    template<typename T>
    nlohmann::json operator()(T &&t);

    nlohmann::json operator()(TPtr<> o);

    template<typename T>
    nlohmann::json operator()(std::vector<T> vec);

    //endregion

    //region DeSerialize

    template<typename T>
    requires std::is_default_constructible_v<T>
    void operator()(const std::function<void(T)> &f, const nlohmann::json &j);

    void operator()(const std::function<void(TPtr<Object>)> &, const nlohmann::json &);

    template<typename T>
    void operator()(const std::function<void(std::vector<T>)> &f, const nlohmann::json &j);
    //endregion

protected:

    nlohmann::json InternalSerialize(const Object *, bool force);

    std::unordered_map<TPtr<const Object>, GUIDFileIDPack> serialize_map{};//to_bimap
    std::unordered_map<GUIDFileIDPack, TPtr<Object>> deserialize_map{};//to_bimap

    std::unordered_multimap<GUIDFileIDPack, std::function<void(TPtr<Object>)>> to_check{};
    std::weak_ptr<int> checking;

    std::shared_ptr<int> register_check();

private:
    nlohmann::json TrueSerialize(const Object *);
};

#include "JsonSerializer.inl"

#endif //RTS_GAME_JSONSERIALIZER_H
