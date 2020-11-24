//
// Created by Michal_Marszalek on 14.09.2020.
//

#include "GameObject.h"
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>

GameObject::GameObject() : scene([&](auto s) {
                                     SceneManager::MoveGameObjectToScene(TPtr<GameObject>{nullptr, shared_from_this()}, s);
                                 },
                                 [&]() { return m_scene; }) {
    AddComponent<Transform>();
}

GameObject::GameObject(std::string name) : GameObject() {
    Object::name = name;
}

bool GameObject::activeSelf() {
    return m_active;
}

void GameObject::initialize_component(TPtr<Component> c) {
    //TODO: !!! Awake of Component when game running !!!
    c->m_gameObject = shared_from_this();
}

TPtr<Transform> GameObject::transform() {
    if (components.empty() || typeid(*components[0].get()) != typeid(Transform)) {
        GameApi::log(ERR.fmt("First component should be transform"));
        std::terminate();
    }

    return components[0];
}
