//
// Created by Michal_Marszalek on 05.01.2021.
//

#ifndef RTS_GAME_SPRITERENDERER_H
#define RTS_GAME_SPRITERENDERER_H


#include "Renderer.h"
#include "Sprite.h"

class SpriteRenderer : public Renderer {
public:

    /// The Sprite to render.
    TPtr<Sprite> sprite;


protected:
    void Awake() override;

    void draw(sf::RenderTarget *) override;
};


#endif //RTS_GAME_SPRITERENDERER_H
