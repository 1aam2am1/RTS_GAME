//
// Created by Michal_Marszalek on 14.01.2021.
//

#ifndef RTS_GAME_POLYGONCOLLIDER2D_H
#define RTS_GAME_POLYGONCOLLIDER2D_H


#include "Collider2D.h"
#include <SFML/System/Vector2.hpp>

class PolygonCollider2D : public Collider2D {
public:

    /// The number of paths in the polygon.
    int pathCount();

    /// Define a path by its constituent points.
    void SetPath(int index, const std::vector<sf::Vector2f> &points);

    /// Gets a path from the Collider by its index.
    std::vector<sf::Vector2f> GetPath(int index);

private:
    std::vector<std::vector<sf::Vector2f>> paths = {{{-0.5, -0.288}, {0.5, -0.288}, {0, 0.577}}};

    std::vector<b2Fixture *> fixtures;

protected:
    void Apply() override;

    void Destroy() override;
};


#endif //RTS_GAME_POLYGONCOLLIDER2D_H
