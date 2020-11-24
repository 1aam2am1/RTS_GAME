//
// Created by Michal_Marszalek on 24.11.2020.
//

#include "SceneSerializer.h"

SceneSerializer::SceneSerializer(std::unordered_map<TPtr<Object> *, nlohmann::json> &bindings) : bind(bindings) {

}

nlohmann::json SceneSerializer::operator()(const TPtr<Object> *ptr) {
    GameApi::log(ERR.fmt("TODO"));
    std::terminate();
    return JsonSerializer::operator()(ptr);
}

void SceneSerializer::operator()(TPtr<Object> *ptr, const nlohmann::json &json) {
    bind[ptr] = json;
}
