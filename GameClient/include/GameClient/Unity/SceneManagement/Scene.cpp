//
// Created by Michal_Marszalek on 08.09.2020.
//

#include "Scene.h"
#include "SceneManager.h"
#include <GameClient/Unity/Core/GameObject.h>

Scene::Scene(uint64_t id) : id(id), name([&](auto s) { SceneManager::data[id].name = s; },
                                         [&]() { return SceneManager::data[id].name; }) {}

bool Scene::isDirty() {
    //TODO: Check if is dirty
    return false;
}

const std::vector<TPtr<GameObject>> &Scene::getRootGameObjects() const noexcept {
    return SceneManager::data[id].root;
}

int Scene::buildIndex() {
    return SceneManager::data[id].buildIndex;
}

bool Scene::isLoaded() {
    return SceneManager::data[id].isLoaded;
}

bool Scene::isValid() {
    return SceneManager::data[id].isValid;
}
