//
// Created by Michal_Marszalek on 15.09.2020.
//

#ifndef RTS_GAME_OBJECT_H
#define RTS_GAME_OBJECT_H

#include <GameApi/Signal.h>
#include <SFML/Window.hpp>
#include <variant>
#include <GameApi/GlobalLogSource.h>
#include <GameApi/Compiler.h>
#include "Quaternion.h"
#include <GameClient/TPtr.h>

class Transform;

/// Base class for all objects Unity can reference.
class Object : public virtual std::enable_shared_from_this<Object> {
public:

    Object();

    Object(const Object &);

    Object &operator=(const Object &);

    virtual ~Object() = default;

    /// Should the object be hidden, saved with the Scene or modifiable by the user?
    ///HideFlags hideFlags;

    /// The name of the object.
    /// \note Components share the same name with the game object and all attached components.
    /// If a class derives from MonoBehaviour it inherits the "name" field from MonoBehaviour.
    /// If this class is also attached to GameObject, then "name" field is set to the name of that GameObject.
    std::string name;

    /// Removes a GameObject, component or asset.
    /// \details The object obj is destroyed immediately after the current Update loop, or t seconds from now if a time
    /// is specified. If obj is a Component, this method removes the component from the GameObject and destroys it.
    /// If obj is a GameObject, it destroys the GameObject, all its components and all transform children of the GameObject.
    /// Actual object destruction is always delayed until after the current Update loop, but is always done before rendering.
    /// \param obj The object to destroy.
    /// \param t The optional amount of time to delay before destroying the object.
    static void Destroy(Object *obj, float t = 0.f);

    /// Destroys the object obj immediately. You are strongly recommended to use Destroy instead.
    /// \details This function should only be used when writing editor code since the delayed destruction will never be
    /// invoked in edit mode. In game code you should use Object.Destroy instead. Destroy is always delayed (but executed
    /// within the same frame). Use this function with care since it can destroy assets permanently! Also note that you
    /// should never iterate through arrays and destroy the elements you are iterating over.
    /// This will cause serious problems (as a general programming practice, not just in Unity).
    /// \param obj Object to be destroyed.
    /// \param allowDestroyingAssets Set to true to allow assets to be destroyed.
    static void DestroyImmediate(TPtr<Object> obj, bool allowDestroyingAssets = false);

    /// Do not destroy the target Object when loading a new Scene.
    /// \details The load of a new Scene destroys all current Scene objects.
    /// Call Object.DontDestroyOnLoad to preserve an Object during level loading.
    /// If the target Object is a component or GameObject, Unity also preserves all of the Transform’s children.
    /// Object.DontDestroyOnLoad does not return a value.
    /// \param target An Object not destroyed on Scene change.
    static void DontDestroyOnLoad(Object target);

    /// Returns the first active loaded object of Type type.
    /// \tparam T The type of object to find.
    /// \return This returns the Object that matches the specified type. It returns null if no Object matches the type.
    template<typename T>
    static std::shared_ptr<T> FindObjectOfType();

    /// Returns a list of all active loaded objects of Type type.
    /// \tparam T The type of object to find.
    /// \return The array of objects found matching the type specified.
    template<typename T>
    static std::vector<std::shared_ptr<T>> FindObjectsOfType();

    /// Clones the object original and returns the clone.
    /// \param original An existing object that you want to make a copy of.
    /// \return The instantiated clone.
    static TPtr<Object> Instantiate(Object *original);

    /// Clones the object original and returns the clone.
    /// \param original An existing object that you want to make a copy of.
    /// \param parent Parent that will be assigned to the new object.
    /// \param instantiateInWorldSpace When you assign a parent Object, pass true to position the new object directly in world space.
    /// Pass false to set the Object’s position relative to its new parent..
    /// \return The instantiated clone.
    static Object Instantiate(Object original, std::shared_ptr<Transform> parent, bool instantiateInWorldSpace = false);

    /// Clones the object original and returns the clone.
    /// \param original An existing object that you want to make a copy of.
    /// \param position Position for the new object.
    /// \param rotation Orientation of the new object.
    /// \param parent Parent that will be assigned to the new object.
    /// \return The instantiated clone.
    static Object
    Instantiate(Object original, sf::Vector3f position, Quaternion rotation, std::shared_ptr<Transform> parent);

    TPtr<Object> shared_from_this();

    TPtr<Object const> shared_from_this() const;

#ifndef TEST
private:
#endif

    // Used in copying Object* when serializing
    mutable sigslot::signal<Object *> onDestroySignal{};

    template<typename>
    friend
    class TPtr;
};


#endif //RTS_GAME_OBJECT_H
