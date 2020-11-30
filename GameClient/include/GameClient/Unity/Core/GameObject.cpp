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

bool GameObject::activeInHierarchy() const {
    /*if (!activeSelf()) { return false; }

    auto parent = transform()->parent.get();
    if (parent.expired()) { return true; }

    return parent->gameObject()->activeInHierarchy();*/

    const GameObject *gm = this;

    while (true) {
        if (!gm->activeSelf()) { return false; }

        // I'm root & i'm active!!! (check is higher)
        auto parent = gm->transform()->parent.get();
        if (parent.expired()) { return true; }

        gm = parent->gameObject().get();
    }

}

bool GameObject::activeSelf() const {
    return m_active;
}

void GameObject::SetActive(bool value) noexcept {
    m_active = value;
}

bool GameObject::CompareTag(std::string_view tag) const noexcept {
    return this->tag == tag;
}

TPtr<Transform> GameObject::transform() const {
    if (components.empty() || typeid(*components[0].get()) != typeid(Transform)) {
        GameApi::log(ERR.fmt("First component should be transform"));
        std::terminate();
    }

    return components[0];
}
