//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Transform.h"
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>

Transform::Transform() :
        parent([&](auto t) {
            SetParent(t, true);
        }, [&]() { return m_parent; }) {

}

Transform::~Transform() {
    for (auto &c: children)
        DestroyImmediate(c.get());
}

TPtr<Transform> Transform::GetChild(size_t index) const {
    if (index >= children.size()) { throw std::runtime_error("GetChild index >= childCount"); }
    return children[index]->transform();
}

int Transform::childCount() {
    return children.size();
}

TPtr<Transform> Transform::Find(std::string_view n) {
    for (auto it : children) {
        if (n == it->gameObject()->name) {
            return it;
        }
    }

    return TPtr<Transform>{nullptr};
}

TPtr<Transform> Transform::root() {
    TPtr<Transform> result = m_parent;

    if (result.expired()) { return TPtr<Transform>{nullptr, shared_from_this()}; }

    while (result->m_parent) {
        result = result->m_parent;
    }

    return result;
}

void Transform::SetParent(const TPtr<Transform> &new_parent, bool worldPositionStays) {
    if (m_parent == new_parent) { return; }
    //TODO: worldPositionStays

    if (m_parent) {
        m_parent->children.erase(std::find_if(m_parent->children.begin(), m_parent->children.end(),
                                              [&](auto &it) {
                                                  return new_parent == it;
                                              }));
    } else {
        auto gm = gameObject();
        auto &objects = SceneManager::data[gm->scene.get()->id].objects;
        objects.erase(
                std::find_if(objects.begin(), objects.end(),
                             [&](auto &it) {
                                 return gm == it;
                             }));
    }

    if (new_parent) {
        new_parent->children.emplace_back(new_parent.get(), shared_from_this());
    } else {
        // add object to root
        SceneManager::data[gameObject()->scene.get()->id].objects.emplace_back(gameObject());
    }

    m_parent = new_parent;
}

void Transform::DetachChildren() {
    auto old_children = children;
    children.clear();

    auto &objects = SceneManager::data[gameObject()->scene.get()->id].objects;
    for (auto it : old_children) {
        if (!it->m_parent) { continue; }

        it->m_parent = nullptr;
        objects.emplace_back(it->gameObject());
    }
}
