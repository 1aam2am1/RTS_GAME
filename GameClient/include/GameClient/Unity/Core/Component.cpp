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
    if (!m_gameObject) { throw std::runtime_error("Wrong m_gameObject in Component"); }

    return TPtr<GameObject>{nullptr, m_gameObject->shared_from_this()};

}

TPtr<Transform> Component::transform() {
    return m_gameObject->transform();
}

bool Component::CompareTag(std::string_view t) const noexcept {
    return tag.get() == t;
}
