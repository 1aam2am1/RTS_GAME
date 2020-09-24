//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_SCRIPTABLEOBJECT_H
#define RTS_GAME_SCRIPTABLEOBJECT_H


#include "Object.h"

/// A class you can derive from if you want to create objects that don't need to be attached to game objects.
/// \details This is most useful for assets which are only meant to store data.
/// To make it easy to create ScriptableObject instances that are bound to assets in your project,
/// see \b CreateAssetMenuAttribute.
class ScriptableObject : public Object {
public:
    /// Creates an instance of a scriptable object.
    /// \details To easily create a ScriptableObject instance that is bound to a .asset
    /// file via the Editor user interface, consider using CreateAssetMenuAttribute.
    /// \tparam T The type of the ScriptableObject to create, as a System.Type instance.
    /// \return The created ScriptableObject.
    template<typename T>
    static std::shared_ptr<ScriptableObject> CreateInstance();
    //TODO: Not shared_ptr as one parent can exists

    /// Awake is called when the script instance is being loaded.
    virtual void Awake();

    /// This function is called when the scriptable object will be destroyed.
    virtual void OnDestroy();

    /// This function is called when the scriptable object goes out of scope.
    virtual void OnDisable();

    /// This function is called when the object is loaded.
    virtual void OnEnable();
};


#endif //RTS_GAME_SCRIPTABLEOBJECT_H
