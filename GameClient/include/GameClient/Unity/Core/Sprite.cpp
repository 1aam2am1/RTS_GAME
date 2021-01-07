//
// Created by Michal_Marszalek on 02.11.2020.
//

#include "Sprite.h"

TPtr<Sprite>
Sprite::Create(TPtr<Texture2D> texture, sf::FloatRect rect, sf::Vector2f pivot, float pixelsPerUnit) {
    TPtr<Sprite> result{std::make_shared<Sprite>()};

    result->texture = texture;
    if (result->texture) { result->s0.setTexture(result->texture->t0); }
    result->s0.setTextureRect(sf::IntRect{rect});

    pivot.x *= result->s0.getLocalBounds().getSize().x;
    pivot.y *= result->s0.getLocalBounds().getSize().y;
    result->s0.setOrigin(pivot);

    result->pixelsPerUnit = pixelsPerUnit;

    return result;
}
