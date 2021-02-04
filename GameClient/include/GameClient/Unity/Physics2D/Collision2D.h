//
// Created by Michal_Marszalek on 18.09.2020.
//

#ifndef RTS_GAME_COLLISION2D_H
#define RTS_GAME_COLLISION2D_H

#include <GameClient/Unity/Core/Transform.h>
#include <GameClient/Unity/Physics2D/Collider2D.h>
#include <GameClient/Unity/Physics2D/Rigidbody2D.h>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

class Collision2D {
public:
    TPtr<Collider2D> collider; //< The incoming Collider2D involved in the collision with the otherCollider.
    //const uint32_t contactCount; //<    Gets the number of contacts for this collision.
    //contacts	The specific points of contact with the incoming Collider2D. You should avoid using this as it produces memory garbage. Use GetContact or GetContacts instead.
    //enabled	Indicates whether the collision response or reaction is enabled or disabled.
    TPtr<GameObject> gameObject; //<	The incoming GameObject involved in the collision.
    TPtr<Collider2D> otherCollider; //<	The other Collider2D involved in the collision with the collider.
    TPtr<Rigidbody2D> otherRigidbody; //<	The other Rigidbody2D involved in the collision with the rigidbody.
    sf::Vector2f relativeVelocity; //<	The relative linear velocity of the two colliding objects (Read Only).
    TPtr<Rigidbody2D> rigidbody; //<	The incoming Rigidbody2D involved in the collision with the otherRigidbody.
    TPtr<Transform> transform; //<	The Transform of the incoming object involved in the collision.


    sf::Vector2f normal;
};


#endif //RTS_GAME_COLLISION2D_H
