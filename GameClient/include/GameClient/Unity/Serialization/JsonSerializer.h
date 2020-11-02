//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_JSONSERIALIZER_H
#define RTS_GAME_JSONSERIALIZER_H

#include "SerializerBase.h"

class JsonSerializer : public SerializerBase {
public:
    nlohmann::json Serialize(const Object *) override;

    template<typename T>
    TPtr<T> DeserializeT(const nlohmann::json &);

    TPtr<Object> Deserialize(std::type_index type, const nlohmann::json &) final;

    using SerializerBase::Deserialize;

protected:
    //region Serialize
    nlohmann::json operator()(const int64_t *) final;

    nlohmann::json operator()(const double *) final;

    nlohmann::json operator()(const std::string *) final;

    nlohmann::json operator()(const bool *) final;

    nlohmann::json operator()(const TPtr<Object> *) override;
    //endregion

    //region DeSerialize
    void operator()(int64_t *, const nlohmann::json &) final;

    void operator()(double *, const nlohmann::json &) final;

    void operator()(std::string *, const nlohmann::json &) final;

    void operator()(bool *, const nlohmann::json &) final;

    void operator()(TPtr<Object> *, const nlohmann::json &) override;
    //endregion
};

template<typename T>
TPtr<T> JsonSerializer::DeserializeT(const nlohmann::json &j) {
    return Deserialize(typeid(T), j);
}


#endif //RTS_GAME_JSONSERIALIZER_H
