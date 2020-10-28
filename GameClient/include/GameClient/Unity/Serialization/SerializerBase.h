//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_SERIALIZERBASE_H
#define RTS_GAME_SERIALIZERBASE_H

#include <GameClient/Unity/Editor/ObjectFactory.h>
#include <GameClient/TPtr.h>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <string>

class SerializerBase {
public:
    /// json -> {'key: TYPE NAME', value: Serialization}
    virtual nlohmann::json Serialize(const Object *) = 0;

    /**template<typename T>
    TPtr<T> Deserialize(const nlohmann::json&);
     */

    /// json -> {value: Serialization from Serialize}
    virtual TPtr<Object> Deserialize(std::type_index type, const nlohmann::json &) = 0;

    /// Deserialize and construct Object
    /// \param str Type in string name
    /// \return Object
    TPtr<Object> Deserialize(std::string_view str, const nlohmann::json &);

protected:
    //region Serialize
    virtual nlohmann::json operator()(const int64_t *) = 0;

    virtual nlohmann::json operator()(const double *) = 0;

    virtual nlohmann::json operator()(const std::string *) = 0;

    virtual nlohmann::json operator()(const TPtr<Object> *) = 0;
    //endregion

    //region DeSerialize
    virtual void operator()(int64_t *, const nlohmann::json &) = 0;

    virtual void operator()(double *, const nlohmann::json &) = 0;

    virtual void operator()(std::string *, const nlohmann::json &) = 0;

    virtual void operator()(TPtr<Object> *, const nlohmann::json &) = 0;
    //endregion

    static std::vector<std::shared_ptr<ObjectFactory::Reflect>> get_reflections(const Object *);

    static decltype(ObjectFactory::reflection)::mapped_type get_name_constructor(std::type_index);

    static std::type_index get_type(std::string_view);
};


#endif //RTS_GAME_SERIALIZERBASE_H
