//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_SERIALIZERBASE_H
#define RTS_GAME_SERIALIZERBASE_H

#include <GameClient/Unity/Core/Object.h>
#include <nlohmann/json.hpp>
#include <typeindex>
#include <cstdint>
#include <string>
#include <GameClient/TPtr.h>
#include <SFML/Graphics/Color.hpp>

template<typename>
class TPtr;

class SerializerBase {
public:
    /// json -> {'TYPE NAME': Serialization}
    virtual nlohmann::json Serialize(const Object *) = 0;

    /// json -> {'TYPE NAME': Serialization}
    TPtr<> Deserialize(const nlohmann::json &);

    /// json -> {value: Serialization from Serialize}
    TPtr<Object> Deserialize(std::type_index type, const nlohmann::json &);

    /// Deserialize and construct Object
    /// \param str Type in string name
    /// \return Object
    TPtr<Object> Deserialize(std::string_view str, const nlohmann::json &);

    /// json -> {value: Serialization from Serialize}
    virtual void Deserialize(TPtr<Object>, const nlohmann::json &) = 0;

protected:
    //region Serialize
    virtual nlohmann::json operator()(int64_t) = 0;

    virtual nlohmann::json operator()(double) = 0;

    virtual nlohmann::json operator()(const std::string &) = 0;

    virtual nlohmann::json operator()(bool) = 0;

    virtual nlohmann::json operator()(sf::Color) = 0;

    virtual nlohmann::json operator()(const TPtr<Object> &) = 0;

    virtual nlohmann::json operator()(const nlohmann::json &);

    virtual nlohmann::json operator()(const std::vector<TPtr<Object>> &) = 0;
    //endregion

    //region DeSerialize
    virtual void operator()(const std::function<void(int64_t)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(double)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(std::string)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(bool)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(sf::Color)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(TPtr<Object>)> &, const nlohmann::json &) = 0;

    virtual void operator()(const std::function<void(nlohmann::json)> &, const nlohmann::json &);

    virtual void operator()(const std::function<void(std::vector<TPtr<Object>>)> &, const nlohmann::json &) = 0;
    //endregion
};


#endif //RTS_GAME_SERIALIZERBASE_H
