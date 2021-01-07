//
// Created by Michal_Marszalek on 05.01.2021.
//

#include "Renderer.h"
#include <GameClient/Unity/Macro.h>

EXPORT_CLASS(Renderer, enabled, forceRenderingOff, sortingOrder)


Renderer::Renderer() : enabled(true), forceRenderingOff(false),
                       sortingOrder([this](int64_t o) {
                           m_sortingOrder = o;
                       }, [this]() -> int64_t {
                           return m_sortingOrder;
                       }),
                       m_sortingOrder(0) {

}
