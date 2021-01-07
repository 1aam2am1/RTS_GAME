//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Transform.h"
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/Macro.h>

namespace sf {
    inline void to_json(nlohmann::json &j, const Vector3f &p) {
        j.clear();
        j["x"] = p.x;
        j["y"] = p.y;
        j["z"] = p.z;
    }

    inline void from_json(const nlohmann::json &j, Vector3f &p) {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
        j.at("z").get_to(p.z);
    }
}

EXPORT_CLASS(Transform, m_localPosition, m_localRotation, m_localScale, m_parent, children);

Transform::Transform() :
        localPosition([&](auto p) { m_localPosition = p; }, [&]() { return m_localPosition; }),
        localRotation([&](auto r) { m_localRotation = r; }, [&]() { return m_localRotation; }),
        localScale([&](auto s) { m_localScale = s; }, [&]() { return m_localScale; }),
        parent([&](auto t) {
            SetParent(t, true);
        }, [&]() { return m_parent; }),
        m_localRotation(0) {

}

Transform::~Transform() {
    for (auto &c: children)
        DestroyImmediate(c);
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

    return {};
}

TPtr<Transform> Transform::root() {
    TPtr<Transform> result = m_parent;

    if (result.expired()) { return TPtr<Transform>(shared_from_this()); }

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
        auto &root = SceneManager::data[gm->scene.get()->id].root;
        root.erase(
                std::find_if(root.begin(), root.end(),
                             [&](auto &it) {
                                 return gm == it;
                             }));
    }

    if (new_parent) {
        new_parent->children.emplace_back(shared_from_this());
    } else {
        // add object to root
        SceneManager::data[gameObject()->scene.get()->id].root.emplace_back(gameObject());
    }

    m_parent = new_parent;
}

void Transform::DetachChildren() {
    auto old_children = children;
    children.clear();

    auto &root = SceneManager::data[gameObject()->scene.get()->id].root;
    for (auto it : old_children) {
        if (!it->m_parent) { continue; }

        it->m_parent = nullptr;
        root.emplace_back(it->gameObject());
    }
}
