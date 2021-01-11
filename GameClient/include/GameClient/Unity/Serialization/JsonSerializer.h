//
// Created by Michal_Marszalek on 28.10.2020.
//

#ifndef RTS_GAME_JSONSERIALIZER_H
#define RTS_GAME_JSONSERIALIZER_H

#include "SerializerBase.h"
#include <GameClient/GuidFileIdPack.h>

class JsonSerializer : public SerializerBase {
public:
    nlohmann::json Serialize(const Object *) override;

    /// json -> {value: Serialization from Serialize}
    template<typename T>
    TPtr<T> DeserializeT(const nlohmann::json &);

    void Deserialize(TPtr<Object>, const nlohmann::json &) override;

    using SerializerBase::Deserialize;

protected:
    //region Serialize
    nlohmann::json operator()(int64_t) final;

    nlohmann::json operator()(double) final;

    nlohmann::json operator()(const std::string &) final;

    nlohmann::json operator()(bool) final;

    nlohmann::json operator()(sf::Color) final;

    nlohmann::json operator()(const TPtr<Object> &) override;

    nlohmann::json operator()(const std::vector<TPtr<Object>> &) override;

    using SerializerBase::operator();
    //endregion

    //region DeSerialize
    void operator()(const std::function<void(int64_t)> &, const nlohmann::json &) final;

    void operator()(const std::function<void(double)> &, const nlohmann::json &) final;

    void operator()(const std::function<void(std::string)> &, const nlohmann::json &) final;

    void operator()(const std::function<void(bool)> &, const nlohmann::json &) final;

    void operator()(const std::function<void(sf::Color)> &, const nlohmann::json &) final;

    void operator()(const std::function<void(TPtr<Object>)> &, const nlohmann::json &) override;

    void operator()(const std::function<void(std::vector<TPtr<Object>>)> &, const nlohmann::json &) override;
    //endregion
};

template<typename T>
TPtr<T> JsonSerializer::DeserializeT(const nlohmann::json &j) {
    return Deserialize(typeid(T), j);
}


#endif //RTS_GAME_JSONSERIALIZER_H
