//
// Created by Michal_Marszalek on 14.09.2020.
//

#include "GameObject.h"
#include "Attributes.h"
#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/MonoBehaviour.h>
#include <GameClient/GlobalStaticVariables.h>
#include <Editor/EditorApplication.h>
#include <utility>

EXPORT_CLASS_CONSTRUCTOR(GameObject, []() { return newGameObject(); }, m_active, components)

TPtr<GameObject> newGameObject() {
    auto i = std::shared_ptr<GameObject>(new GameObject());
    i->scene = SceneManager::GetActiveScene();
    //auto _ = i->transform(); ///< Create transform //Fix components as serializing was overwriting transform without destroying

    return i;
}

TPtr<GameObject> newGameObject(std::string name) {
    auto i = std::shared_ptr<GameObject>(new GameObject(std::move(name)));
    i->scene = SceneManager::GetActiveScene();
    //auto _ = i->transform(); ///< Create transform

    return i;
}
////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject() : scene([&](auto s) {
                                     SceneManager::MoveGameObjectToScene(TPtr<GameObject>(shared_from_this()), s);
                                 },
                                 [&]() { return m_scene; }) {
    Object::name = "GameObject";
}

GameObject::GameObject(std::string name) : GameObject() {
    Object::name = std::move(name);
}

GameObject::~GameObject() {
    for (auto &c: components)
        DestroyImmediate(c);
}

bool GameObject::activeInHierarchy() const {
    /*if (!activeSelf()) { return false; }

    auto parent = transform()->parent.get();
    if (parent.expired()) { return true; }

    return parent->gameObject()->activeInHierarchy();*/

//TODO: Make it vaster by caching result!

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
    bool was_active_in_h = activeInHierarchy();

    if (m_active != value) {
        m_active = value;

        if (!m_scene) { return; } //this is prefab
        assert(m_scene->isLoaded()); //should only be called on loaded scene

        bool now_active_in_h = activeInHierarchy();
        if (now_active_in_h) {
            decltype(to_awake) copy;
            std::swap(copy, to_awake);

            bool isPlaying = EditorApplication::isPlaying;
            for (auto it = copy.begin(); it != copy.end(); ++it) {
                if (!*it) { continue; }

                if ((isPlaying || Attributes::CheckCustomAttribute(*it, ExecuteInEditMode))) {
                    (*it)->UnityAwake();
                    //result->UnityOnActiveChange(true); later in now_active_in_h != was_active_in_h
                    global.scene.data[m_scene->id].new_components.emplace_back(*it); //To Start()
                } else {
                    // Don't add, reload scene
                }
            }
        }

        if (now_active_in_h != was_active_in_h) {
            for (auto &c : components) {
                c->UnityOnActiveChange(now_active_in_h);
            }
        }
    }
}

bool GameObject::CompareTag(std::string_view t) const noexcept {
    return this->tag == t;
}

TPtr<Transform> GameObject::transform() const {
    if (components.empty()) {
        const_cast<GameObject *>(this)->AddComponent<Transform>();
    }

    auto result = dynamic_pointer_cast<Transform>(components[0]); //Can be static_pointer_cast

    if (components.empty() || result.expired()) {
        GameApi::log(ERR.fmt("First component should be transform"));
        std::terminate();
    }

    return result;
}

TPtr<Component> GameObject::AddComponent(std::type_index type) {
    auto reflection = MetaData::getReflection(type);
    auto result = reflection.CreateInstance();
    return AddComponent(result);
}

TPtr<Component> GameObject::AddComponent(TPtr<Component> result) {
    if (result.expired()) { return {}; }
    {
        auto t = dynamic_pointer_cast<Transform>(result);
        if (t && !components.empty()) {
            GameApi::log(ERR.fmt("Transform should be one for game object"));
            return transform();
        }
        if (!t && components.empty()) {
            auto discard = transform(); //< Adds transform to game object
        }
    }

    if (Attributes::CheckCustomAttribute(result, DisallowMultipleComponent)) {
        auto reflection = MetaData::getReflection(result.get());

        for (auto c : components) {
            if (reflection.CheckInstance(c.get())) {
                return c;
            }
        }
    }

    //TODO: Check meta if there is flag only one and then check if there exists
    components.emplace_back(result);

    //TODO: !!! Awake of Component when game running !!!
    IM_ASSERT(result->m_gameObject == nullptr);
    result->m_gameObject = static_pointer_cast<GameObject>(shared_from_this());

    if (!m_scene) { return result; } //this is prefab, don't call Awake or Start

    if (!activeInHierarchy()) {
        to_awake.emplace_back(result);
    } else {
        if (EditorApplication::isPlaying || Attributes::CheckCustomAttribute(result, ExecuteInEditMode)) {
            if (m_scene->isLoaded()) {
                result->UnityAwake();
                result->UnityOnActiveChange(true);
                global.scene.data[m_scene->id].new_components.emplace_back(result); //To Start()
            } else {
                global.scene.data[m_scene->id].loading_awake.emplace_back(result); //To Awake()
            }
        } else {
            // Don't add, reload scene
        }
    }

    return result;
}
