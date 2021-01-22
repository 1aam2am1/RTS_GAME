//
// Created by Michal_Marszalek on 02.11.2020.
//

#include <Macro.h>
#include "Sprite.h"

EXPORT_CLASS(Sprite)

Sprite::Sprite() {
    texture.on_change.connect([&](auto) {
        if (texture) {
            s0.setTexture(texture->t0);
        } else {
            sf::Texture *t0 = nullptr;
            s0.setTexture(*t0);//Reverence must be null as sprite is now null.
        }
    });
}

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
