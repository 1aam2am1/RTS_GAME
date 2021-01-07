//
// Created by Michal_Marszalek on 05.01.2021.
//

#ifndef RTS_GAME_RENDERER_H
#define RTS_GAME_RENDERER_H


#include "Component.h"

class Renderer : public Component {
public:
    Renderer();

    /// Makes the rendered 3D object visible if enabled.
    bool enabled;

    /// Allows turning off rendering for a specific component.
    bool forceRenderingOff;

    /// Renderer's order within a sorting layer.
    SetterGetter<int64_t> sortingOrder;

private:
    friend class Camera;

    int64_t m_sortingOrder;

protected:
    virtual void draw() = 0;
};


#endif //RTS_GAME_RENDERER_H
