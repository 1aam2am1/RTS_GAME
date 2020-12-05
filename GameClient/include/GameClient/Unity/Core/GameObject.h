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

class GameObject;

TPtr<GameObject> newGameObject();

TPtr<GameObject> newGameObject(std::string);

/// Creates a new game object, named \b name.
/// \tparam Args A list of Components to add to the GameObject on creation.
/// \param name The name that the GameObject is created with.
template<typename... Args>
TPtr<GameObject> newGameObject(std::string name);

/// Base class for all entities in Unity Scenes.
class GameObject : public Object {
    friend TPtr<GameObject> newGameObject();

    friend TPtr<GameObject> newGameObject(std::string);

    template<typename... Args>
    friend TPtr<GameObject> newGameObject(std::string);

    GameObject();

    /// Creates a new game object, named \b name.
    /// \param name The name that the GameObject is created with.
    explicit GameObject(std::string name);

public:
    ~GameObject();

    /// Defines whether the GameObject is active in the Scene.
    /// \note This lets you know whether a GameObject is active in the game.
    /// That is the case if its GameObject.activeSelf property is enabled, as well as that of all its parents.
    [[nodiscard]] bool activeInHierarchy() const;

    /// The local active state of this GameObject
    [[nodiscard]] bool activeSelf() const;

    /// The layer the game object is in.
    /// \note Layers can be used for selective rendering from cameras or ignoring raycasts.
    int64_t layer = 0;

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
    TPtr<Scene> m_scene{};
};

#include "GameObject.inl"

#endif //RTS_GAME_GAMEOBJECT_H
