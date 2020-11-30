//
// Created by Michal_Marszalek on 24.11.2020.
//

#ifndef RTS_GAME_SCENESERIALIZER_H
#define RTS_GAME_SCENESERIALIZER_H

#include <GameClient/Unity/Serialization/JsonSerializer.h>


class SceneSerializer final : public JsonSerializer {
public:
    SceneSerializer(std::unordered_map<TPtr<Object>, nlohmann::json> &bindings);

protected:
    nlohmann::json operator()(const TPtr<Object> &ptr) override;

    void operator()(const std::function<void(TPtr<Object>)> &ptr, const nlohmann::json &json) override;

    std::unordered_map<TPtr<Object>, nlohmann::json> &bind;
};


#endif //RTS_GAME_SCENESERIALIZER_H
