//
// Created by Michal_Marszalek on 05.01.2021.
//

#ifndef RTS_GAME_EDITORGUILAYOUT_H
#define RTS_GAME_EDITORGUILAYOUT_H

#include <GameClient/TPtr.h>
#include <typeindex>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class EditorGUILayout {
public:

    /// Make a field to receive any object type.
    /// You can assign objects either by drag and drop or by selecting an object using the Object Picker.
    /// \param obj The object the field shows.
    /// \param objType The type of the objects that can be assigned.
    /// \param allowSceneObjects Allow assigning Scene objects. See Description for more info.
    /// \return The object that has been set by the user.
    static TPtr<> ObjectField(TPtr<> obj, std::type_index objType, bool allowSceneObjects);

    static sf::FloatRect RectField(sf::FloatRect);

    static sf::IntRect RectField(sf::IntRect);

    static sf::Vector2i Vector2Field(sf::Vector2i);

    static sf::Vector2f Vector2Field(sf::Vector2f);

    static sf::Vector3i Vector3Field(sf::Vector3i);

    static sf::Vector3f Vector3Field(sf::Vector3f);
};


#endif //RTS_GAME_EDITORGUILAYOUT_H
