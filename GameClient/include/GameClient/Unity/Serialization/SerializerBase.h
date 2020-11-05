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

template<typename>
class TPtr;

class SerializerBase {
public:
    /// json -> {'key: TYPE NAME', value: Serialization}
    virtual nlohmann::json Serialize(const Object *) = 0;

    /**template<typename T>
    TPtr<T> Deserialize(const nlohmann::json&);
     */

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
    virtual nlohmann::json operator()(const int64_t *) = 0;

    virtual nlohmann::json operator()(const double *) = 0;

    virtual nlohmann::json operator()(const std::string *) = 0;

    virtual nlohmann::json operator()(const bool *) = 0;

    virtual nlohmann::json operator()(const TPtr<Object> *) = 0;

    nlohmann::json operator()(const nlohmann::json *);
    //endregion

    //region DeSerialize
    virtual void operator()(int64_t *, const nlohmann::json &) = 0;

    virtual void operator()(double *, const nlohmann::json &) = 0;

    virtual void operator()(std::string *, const nlohmann::json &) = 0;

    virtual void operator()(bool *, const nlohmann::json &) = 0;

    virtual void operator()(TPtr<Object> *, const nlohmann::json &) = 0;

    void operator()(nlohmann::json *, const nlohmann::json &);
    //endregion
};


#endif //RTS_GAME_SERIALIZERBASE_H
