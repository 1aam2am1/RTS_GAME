//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Transform.h"
#include "Attributes.h"
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/Unity/Serialization/to_json.h>

EXPORT_CLASS(Transform, ("m_localPosition", localPosition), ("m_localRotation", localRotation),
             ("m_localScale", localScale), ("m_parent", parent), children);
ADD_ATTRIBUTE(Transform, ExecuteInEditMode)

Transform::Transform() :
        localPosition(this, &Transform::OnPositionChange),
        localRotation(this, &Transform::OnPositionChange, 0),
        localScale(this, &Transform::OnPositionChange, 1.f, 1.f, 1.f),
        parent([&](auto t) {
            SetParent(t, true);
        }, [&]() { return m_parent; }) {

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
        auto it = std::find_if(m_parent->children.begin(), m_parent->children.end(),
                               [&](auto &it) {
                                   return new_parent == it;
                               });
        if (it != m_parent->children.end()) {
            m_parent->children.erase(it);
        } else {
            assert(0); //should not happen, that we have parent and he don't know anything about it
        }
    } else {
        auto gm = gameObject();
        auto &root = global.scene.data[gm->scene.get()->id].root;
        auto it = std::find_if(root.begin(), root.end(),
                               [&](auto &it) {
                                   return gm == it;
                               });
        if (it != root.end()) { root.erase(it); }
    }

    if (new_parent) {
        new_parent->children.emplace_back(shared_from_this());
    } else {
        // add object to root
        global.scene.data[gameObject()->scene.get()->id].root.emplace_back(gameObject());
    }

    m_parent = new_parent;
}

void Transform::DetachChildren() {
    auto old_children = children;
    children.clear();

    auto &root = global.scene.data[gameObject()->scene.get()->id].root;
    for (auto &&it : old_children) {
        if (!it->m_parent) { continue; }

        it->m_parent = nullptr;
        root.emplace_back(it->gameObject());
    }
}

void Transform::OnPositionChange() {
    localRotation.t -= 360.0f * floorf(localRotation.t / 360.0f);
    if (m_unityAwakeed && (m_physics_root || m_colliders) && !global.physics.transform_lock && !m_dirty_registered) {
        //Add to Transform vector box2d
        m_dirty_registered = true;
        global.physics.transform_dirty.emplace_back(static_pointer_cast<Transform>(shared_from_this()));
    }
}
