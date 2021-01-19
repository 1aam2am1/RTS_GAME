//
// Created by Michal_Marszalek on 15.09.2020.
//

#include "Component.h"
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/Core/Transform.h>

Component::Component() :
        tag([&](auto t) { m_gameObject->tag = t; }, [&]() { return m_gameObject->tag; }) {

}

Component::~Component() = default;

TPtr<GameObject> Component::gameObject() const {
    return m_gameObject;

}

TPtr<Transform> Component::transform() {
    return m_transform;
}

bool Component::CompareTag(std::string_view t) const noexcept {
    return tag.get() == t;
}
