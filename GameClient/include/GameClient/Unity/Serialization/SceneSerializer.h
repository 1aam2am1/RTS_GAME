//
// Created by Michal_Marszalek on 24.11.2020.
//

#ifndef RTS_GAME_SCENESERIALIZER_H
#define RTS_GAME_SCENESERIALIZER_H

#include <GameClient/Unity/Serialization/JsonSerializer.h>
#include <GameClient/GuidFileIdPack.h>


class SceneSerializer final : public JsonSerializer {
public:
    SceneSerializer();

    nlohmann::json Serialize(const Object *object) override;

    nlohmann::json Serialize(const std::vector<TPtr<>> &objects);

    void Deserialize(TPtr<Object>, const nlohmann::json &) override;

    void Deserialize(std::vector<TPtr<>> &, const nlohmann::json &);

    /// Callback that will be called when we need id for our object
    std::function<std::pair<GUIDFileIDPack, bool>(TPtr<const Object>)> callback_id{};

    /// Callback that will be called when we need object from this json id
    std::function<TPtr<>(GUIDFileIDPack)> ret_callback_id{};

    const std::unordered_map<TPtr<const Object>, GUIDFileIDPack> &get_id_map();

protected:
    void operator()(const std::function<void(TPtr<Object>)> &, const nlohmann::json &) final;

    nlohmann::json operator()(const std::vector<TPtr<Object>> &) final;

private:
    std::unordered_map<TPtr<const Object>, GUIDFileIDPack> serialize_map{};
};


#endif //RTS_GAME_SCENESERIALIZER_H
