//
// Created by Michal_Marszalek on 15.09.2020.
//

#ifndef RTS_GAME_COMPONENT_H
#define RTS_GAME_COMPONENT_H


#include "Object.h"
#include <GameClient/TPtr.h>
#include <GameApi/SetterGetterF.h>
#include <GameClient/Unity/Core/GameObject.h>

class Transform;

/// Base class for everything attached to GameObjects.
class Component : public Object {
public:

    Component();

    ~Component() override;

    /// The game object this component is attached to. A component is always attached to a game object.
    [[nodiscard]] TPtr<GameObject> gameObject() const;

    /// The tag of this game object.
    SetterGetterF<std::string> tag;

    /// The Transform attached to this GameObject.
    TPtr<Transform> transform();

    /// Is this game object tagged with tag ?
    /// \param tag The tag to compare.
    /// \return Returns true if the same
    [[nodiscard]] bool CompareTag(std::string_view tag) const noexcept;

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
    [[nodiscard]] std::vector<TPtr<T>> GetComponents() const;

    /// Returns all components of Type type in the GameObject or any of its children.
    /// \tparam T The type of Component to retrieve.
    /// \param includeInactive Should Components on inactive GameObjects be included in the found set?
    /// \return Array of components
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    [[nodiscard]] std::vector<TPtr<T>> GetComponentsInChildren(bool includeInactive = false) const;

    /// Returns all components of Type type in the GameObject or any of its parents.
    /// \tparam T The type of Component to retrieve.
    /// \param includeInactive Should inactive Components be included in the found set?
    /// \return
    template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
    [[nodiscard]] std::vector<TPtr<T>> GetComponentsInParent(bool includeInactive = false) const;

private:
    friend class GameObject;

    friend class GameLoop;

    friend class SceneLoader;

    friend class Object;

    TPtr<GameObject> m_gameObject{};

    TPtr<Transform> m_transform{};
protected:
    //TODO:: Make it private in MonoBehaviour

#define ONE_TIME_EXEC(NAME) void Unity##NAME(){if(!m_unity##NAME##ed){m_unity##NAME##ed=true;NAME();}}bool m_unity##NAME##ed = false; virtual void NAME(){}

    void UnityAwake();

    bool m_unityAwakeed = false;

    virtual void Awake() {}

    ONE_TIME_EXEC(Start)

#undef ONE_TIME_EXEC

    virtual void UnityOnActiveChange(bool) = 0; //Emits OnEnable, OnDisable

    virtual void OnDestroy() {}
};

#include "Component.inl"


#endif //RTS_GAME_COMPONENT_H
