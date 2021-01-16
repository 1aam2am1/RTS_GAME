//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "Renderer.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(Renderer, ("m_enabled", enabled), forceRenderingOff, sortingOrder)


Renderer::Renderer() : enabled(true), forceRenderingOff(false),
                       sortingOrder([this](int64_t o) {
                           m_sortingOrder = o;
                       }, [this]() -> int64_t {
                           return m_sortingOrder;
                       }),
                       m_sortingOrder(0) {

}

void Renderer::UnityOnActiveChange(bool b) {
    m_onEnable = b & enabled;
}
