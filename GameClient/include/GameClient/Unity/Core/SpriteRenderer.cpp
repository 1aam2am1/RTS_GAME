//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "SpriteRenderer.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/GlobalStaticVariables.h>

ADD_COMPONENT_MENU(SpriteRenderer, sprite)

void SpriteRenderer::UnityAwake() {
    //TODO: Move to UnityUpdate
    global.m_render.emplace_back(shared_from_this());
}

void SpriteRenderer::draw() {
    if (!sprite) { return; }

    auto position = transform()->localPosition.get();

    sf::RenderStates states;
    double pixelsPerUnit = sprite->pixelsPerUnit;
    pixelsPerUnit = 1.0 / pixelsPerUnit;

    states.transform.translate(position.x, position.y);
    states.transform.rotate(transform()->localRotation);

    states.transform.scale(transform()->localScale.get().x * pixelsPerUnit,
                           transform()->localScale.get().y * pixelsPerUnit);

    global.m_target.draw(sprite->s0, states);
}
