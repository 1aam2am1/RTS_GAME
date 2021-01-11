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
    static TPtr<Component> AddComponent(TPtr<GameObject> gameObject, std::type_info type);

    template<typename T>
    static TPtr<T> AddComponent(TPtr<GameObject> gameObject);

    /// Creates a new GameObject.
    static TPtr<GameObject> CreateGameObject(std::string name, std::vector<std::type_info> types);

    /// Create a new instance of the given type.
    static TPtr<> CreateInstance(std::type_info type);

    /// Create a new instance of the given type.
    template<typename T>
    static TPtr<T> CreateInstance();

};


#endif //RTS_GAME_OBJECTFACTORY_H
