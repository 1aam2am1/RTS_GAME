//
// Created by Michal_Marszalek on 27.10.2020.
//

#ifndef RTS_GAME_OBJECTFACTORY_H
#define RTS_GAME_OBJECTFACTORY_H

#include <GameClient/Unity/Core/GameObject.h>
#include <typeinfo>
#include <vector>
#include <typeindex>

class ObjectFactory {
public:

    /// Creates a new component and adds it to the specified GameObject.
    static Component *AddComponent(GameObject *gameObject, std::type_info type);

    template<typename T>
    static Component *AddComponent(GameObject *gameObject);

    /// Creates a new GameObject.
    static GameObject *CreateGameObject(std::string name, std::vector<std::type_info> types);

    /// Create a new instance of the given type.
    static Object *CreateInstance(std::type_info type);

    template<typename T>
    static Object *CreateInstance();

    /// Create a new instance of the given type.
    template<typename T>
    static T *CreateInstance();

};


#endif //RTS_GAME_OBJECTFACTORY_H
