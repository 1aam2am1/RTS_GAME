//
// Created by Michal_Marszalek on 2021-01-24.
//

#ifndef RTS_GAME_PHYSICS2D_H
#define RTS_GAME_PHYSICS2D_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <Physics2D/Collider2D.h>

class Physics2D {
public:
    /// Get a list of all colliders that fall within a box area.
    /// \param point The center of the box.
    /// \param size The size of the box.
    /// \return
    static std::vector<TPtr<Collider2D>> OverlapBoxAll(sf::Vector2f point, sf::Vector2f size);

    static std::vector<TPtr<Collider2D>> OverlapCircleAll(sf::Vector2f point, float radius);

    static TPtr<Collider2D> OverlapPoint(sf::Vector2f point);
};


#endif //RTS_GAME_PHYSICS2D_H
