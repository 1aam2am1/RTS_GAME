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

static const std::vector<TPtr<GameObject>> empty{};

const std::vector<TPtr<GameObject>> &Scene::getRootGameObjects() const noexcept {
    auto it = SceneManager::data.find(id);
    if (it != SceneManager::data.end()) {
        return it->second.root;
    }

    return empty;
}

int Scene::buildIndex() {
    auto it = SceneManager::data.find(id);
    if (it != SceneManager::data.end()) {
        return it->second.buildIndex;
    }
    return -1;
}

bool Scene::isLoaded() {
    auto it = SceneManager::data.find(id);
    if (it != SceneManager::data.end()) {
        return it->second.isLoaded;
    }
    return false;
}

bool Scene::isValid() {
    return SceneManager::data.find(id) != SceneManager::data.end();
}
