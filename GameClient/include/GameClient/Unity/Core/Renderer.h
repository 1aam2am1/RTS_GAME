//
// Created by Michal_Marszalek on 05.01.2021.
//

#ifndef RTS_GAME_RENDERER_H
#define RTS_GAME_RENDERER_H


#include <SFML/Graphics/RenderTarget.hpp>
#include "Component.h"
#include <GameApi/SetterEmitterP.h>

class Renderer : public Component {
public:
    Renderer();

    /// Makes the rendered 3D object visible if enabled.
    SetterEmitterP<bool, Renderer> enabled;

    /// Allows turning off rendering for a specific component.
    bool forceRenderingOff;

    /// Renderer's order within a sorting layer.
    SetterGetterF<int64_t> sortingOrder;

private:
    friend class Camera;

    friend class SceneWindow; //remove this

    int64_t m_sortingOrder;

    bool m_onEnable = false;
    bool m_gameObjectCacheEnabled = false;

    void UnityOnActiveChange(bool) override;

    void EnableChange();

protected:
    virtual void draw(sf::RenderTarget * = nullptr) = 0;
};


#endif //RTS_GAME_RENDERER_H
