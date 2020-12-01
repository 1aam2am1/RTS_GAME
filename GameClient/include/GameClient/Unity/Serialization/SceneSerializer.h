//
// Created by Michal_Marszalek on 24.11.2020.
//

#ifndef RTS_GAME_SCENESERIALIZER_H
#define RTS_GAME_SCENESERIALIZER_H

#include <GameClient/Unity/Serialization/JsonSerializer.h>
#include <GameClient/GuidFileIdPack.h>


class SceneSerializer final : public JsonSerializer {
public:
    explicit SceneSerializer();

protected:
    nlohmann::json operator()(const TPtr<Object> &ptr) override;

    void operator()(const std::function<void(TPtr<Object>)> &ptr, const nlohmann::json &json) override;

    void operator()(const std::function<void(std::vector<TPtr<Object>>)> &, const nlohmann::json &) override;

public:
    std::vector<std::pair<std::function<void(TPtr<Object>)>, GUIDFileIDPack>> bind;
    std::vector<std::pair<std::function<void(std::vector<TPtr<Object>>)>, nlohmann::json>> bind_vector;

    std::unordered_map<TPtr<Object>, GUIDFileIDPack> serialize_map;
    Unity::fileID max_id = 0;
};


#endif //RTS_GAME_SCENESERIALIZER_H
