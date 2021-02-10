//
// Created by Michal_Marszalek on 24.11.2020.
//

#ifndef RTS_GAME_SCENESERIALIZER_H
#define RTS_GAME_SCENESERIALIZER_H

#include <GameClient/Unity/Serialization/JsonSerializer.h>


class SceneSerializer final : public JsonSerializer {
public:
    nlohmann::json Serialize(const std::vector<TPtr<>> &);

    void Deserialize(std::vector<TPtr<>> &, const nlohmann::json &);

    Unity::GUID zero;
protected:
    std::pair<GUIDFileIDPack, bool> serialize_node_callback(TPtr<const Object> ptr) override;

    TPtr<> deserialize_get_node_callback(GUIDFileIDPack pack) override;

private:
    uint64_t max_id = 0;
};


#endif //RTS_GAME_SCENESERIALIZER_H
