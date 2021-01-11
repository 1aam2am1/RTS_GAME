//
// Created by Michal_Marszalek on 02.11.2020.
//

#ifndef RTS_GAME_SPRITE_H
#define RTS_GAME_SPRITE_H

#include <GameClient/Unity/Core/Texture2D.h>
#include <SFML/Graphics/Sprite.hpp>
#include <GameClient/TPtr.h>


class Sprite : public Object {
public:
    Sprite();

    sf::Sprite s0;

    uint32_t pixelsPerUnit = 32;

    static TPtr<Sprite>
    Create(TPtr<Texture2D> texture, sf::FloatRect rect, sf::Vector2f pivot = {0.5, 0.5}, float pixelsPerUnit = 32);

private:
    TPtr<Texture2D> texture{};
};


#endif //RTS_GAME_SPRITE_H
