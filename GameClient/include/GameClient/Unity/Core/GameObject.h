//
// Created by Michal_Marszalek on 14.09.2020.
//

#ifndef RTS_GAME_GAMEOBJECT_H
#define RTS_GAME_GAMEOBJECT_H


#include <string>
#include <GameClient/TPtr.h>
#include <GameApi/SetterGetter.h>
#include <GameClient/Unity/SceneManagement/Scene.h>
#include <GameClient/Unity/Editor/Application.h>
#include <GameApi/has_field.h>

class Component;

class Transform;

/// Base class for all entities in Unity Scenes.
class GameObject : public Object {
public:
    /// Creates a new game object, named \b name.
    GameObject();

    /// Creates a new game object, named \b name.
    /// \param name The name that the GameObject is created with.
    GameObject(std::string name);

    /// Creates a new game object, named \b name.
    /// \tparam Args A list of Components to add to the GameObject on creation.
    /// \param name The name that the GameObject is created with.
    template<typename... Args, std::enable_if_t<std::conjunction_v<std::is_base_of<Component, Args>...>, int> = 0>
    GameObject(std::string name);

    /// Defines whether the GameObject is active in the Scene.
    /// \note This lets you know whether a GameObject is active in the game.
    /// That is the case if its GameObject.activeSelf property is enabled, as well as that of all its parents.
    bool activeInHierarchy() const;

    /// The local active state of this GameObject
    bool activeSelf() const;

    /// The layer the game object is in.
    /// \note Layers can be used for selective rendering from cameras or ignoring raycasts.
    int layer;

    /// Scene that the GameObject is part of.
    SetterGetter<TPtr<Scene>> scene;

    /// The tag of this game object.
    std::string tag; ///< TODO: Change for enum class with int and string Layers manager

    /// The Transform attached to this GameObject.
    [[nodiscard]] TPtr<Transform> transform() const;

    /// Adds a component class of type componentType to the game object.
    /// \note Note that there is no RemoveComponent(), to remove a component, use Object.Destroy.
    /// \tparam T The type of Component to create.
    /// \return Component
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    TPtr<T> AddComponent();

    /// Is this game object tagged with tag ?
    /// \param tag The tag to compare.
    /// \return Returns true if the same
    bool CompareTag(std::string_view tag) const noexcept;

    /// Returns the component of Type type if the game object has one attached, null if it doesn't.
    /// \tparam T The type of Component to retrieve.
    /// \return Component A component of the matching type, if found.
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    TPtr<T> GetComponent() const;

    /// Returns the component of Type type in the GameObject or any of its children using depth first search.
    /// \note A component is returned only if it is found on an active GameObject.
    /// \tparam T The type of Component to retrieve.
    /// \return Component A component of the matching type, if found.
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    TPtr<T> GetComponentInChildren() const;

    /// Retrieves the component of Type type in the GameObject or any of its parents.
    /// \tparam T Type of component to find.
    /// \return Component Returns a component if a component matching the type is found. Returns null otherwise.
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    TPtr<T> GetComponentInParent() const;

    /// Returns all components of Type type in the GameObject.
    /// \tparam T The type of component to retrieve.
    /// \return Array of components
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    std::vector<TPtr<T>> GetComponents() const;

    /// Returns all components of Type type in the GameObject or any of its children.
    /// \tparam T The type of Component to retrieve.
    /// \param includeInactive Should Components on inactive GameObjects be included in the found set?
    /// \return Array of components
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    std::vector<TPtr<T>> GetComponentsInChildren(bool includeInactive = false) const;

    /// Returns all components of Type type in the GameObject or any of its parents.
    /// \tparam T The type of Component to retrieve.
    /// \param includeInactive Should inactive Components be included in the found set?
    /// \return
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    std::vector<TPtr<T>> GetComponentsInParent(bool includeInactive = false) const;

    /// Activates/Deactivates the GameObject, depending on the given true or false value.
    /// \param value Activate or deactivate the object, where true activates the GameObject and false deactivates the GameObject.
    void SetActive(bool value) noexcept;

    /// Finds a GameObject by name and returns it in all scenes.
    /// \details This function only returns active GameObjects. If no GameObject with name can be found,
    /// null is returned. If name contains a '/' character, it traverses the hierarchy like a path name.
    /// \param name Name of the object
    /// \return TPtr<GameObject>
    static auto Find(std::string_view name);

    /// Returns an array of active GameObjects tagged tag. Returns empty array if no GameObject was found.
    /// \param tag The name of the tag to search GameObjects for.
    /// \return Array of GameObjects
    static std::vector<TPtr<GameObject>> FindGameObjectsWithTag(std::string_view tag);

    /// Returns one active GameObject tagged tag. Returns null if no GameObject was found.
    /// \param tag The tag to search for.
    /// \return GameObject
    static TPtr<GameObject> FindWithTag(std::string_view tag);

private:
    friend class SceneManager;

    bool m_active = true;

    //TODO: Start new component before running
    std::vector<TPtr<Component>> components;
    std::unordered_map<TPtr<Component>, std::function<void()>> to_awake;
    TPtr<Scene> m_scene{this};
};

HAS_FIELD(Awake)

template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
TPtr<T> GameObject::AddComponent() {
    if constexpr (std::is_base_of_v<Transform, T>) {
        if (!components.empty() && typeid(*components[0].get()) != typeid(Transform)) {
            GameApi::log(ERR.fmt("First component should be transform"));
        }
    }
    //TODO: Check meta if there is flag only one and then check if there exists
    auto result = components.emplace_back(this, std::make_shared<T>());

    //TODO: !!! Awake of Component when game running !!!
    result->m_gameObject = shared_from_this();

    if constexpr (has_fieldAwake_v<T>) {
        auto pointer = result.get();
        auto f = [pointer]() {
            static_cast<T *>(pointer)->Awake();
        };

        if (Application::isPlaying && activeInHierarchy()) {
            f();
        } else {
            to_awake.emplace(result, f);
        }
    }

    return result;
}


#endif //RTS_GAME_GAMEOBJECT_H
