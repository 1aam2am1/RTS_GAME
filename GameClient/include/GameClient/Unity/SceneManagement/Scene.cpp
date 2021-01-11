//
// Created by Michal_Marszalek on 08.09.2020.
//

#include "Scene.h"
#include "SceneManager.h"
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/GlobalStaticVariables.h>

Scene::Scene(uint64_t id) : id(id), name([&](auto s) { if (isValid()) global.scene.data[id].name = s; },
                                         [&]() { return isValid() ? global.scene.data[id].name : std::string{}; }) {}

bool Scene::isDirty() {
    //TODO: Check if is dirty
    return false;
}

static const std::vector<TPtr<GameObject>> empty{};

const std::vector<TPtr<GameObject>> &Scene::getRootGameObjects() const noexcept {
    auto it = global.scene.data.find(id);
    if (it != global.scene.data.end()) {
        return it->second.root;
    }

    return empty;
}

int Scene::buildIndex() {
    auto it = global.scene.data.find(id);
    if (it != global.scene.data.end()) {
        return it->second.buildIndex;
    }
    return -1;
}

bool Scene::isLoaded() {
    auto it = global.scene.data.find(id);
    if (it != global.scene.data.end()) {
        return it->second.isLoaded;
    }
    return false;
}

bool Scene::isValid() {
    return global.scene.data.find(id) != global.scene.data.end();
}

std::string Scene::path() {
    auto it = global.scene.data.find(id);
    if (it != global.scene.data.end()) {
        return it->second.path;
    }
    return {};
}
