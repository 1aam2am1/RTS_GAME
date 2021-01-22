//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "Renderer.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Transform.h>

EXPORT_CLASS(Renderer, ("m_enabled", enabled), forceRenderingOff, sortingOrder)


Renderer::Renderer() : enabled(this, EnableChange, true), forceRenderingOff(false),
                       sortingOrder([this](int64_t o) {
                           m_sortingOrder = o;
                       }, [this]() -> int64_t {
                           return m_sortingOrder;
                       }),
                       m_sortingOrder(0) {

}

void Renderer::UnityOnActiveChange(bool b) {
    m_gameObjectCacheEnabled = b;

    m_onEnable = m_gameObjectCacheEnabled & enabled;
}

void Renderer::EnableChange() {
    m_onEnable = m_gameObjectCacheEnabled & enabled;
};
