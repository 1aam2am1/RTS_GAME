//
// Created by Michal_Marszalek on 14.12.2020.
//

#include "Camera.h"
#include <GameClient/GlobalStaticVariables.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Editor/Selection.h>

ADD_COMPONENT_MENU(Camera)

Camera::Camera() : depth(
        [&](auto d) {
            m_depth = d;
            if (m_draw_order_it != global.m_draw_order.end()) { global.m_draw_order.erase(m_draw_order_it); }
            m_draw_order_it = global.m_draw_order.emplace(m_depth, this);

        }, [&]() { return m_depth; }) {

    m_draw_order_it = global.m_draw_order.end();
}

void Camera::internalAwake() {
    global.m_cameras.emplace_back(this);
    m_draw_order_it = global.m_draw_order.emplace(m_depth, this);
}

Camera::~Camera() {
    global.m_cameras.erase(
            std::find_if(global.m_cameras.begin(), global.m_cameras.end(), [&](auto it) { return it == this; }));
    if (m_draw_order_it != global.m_draw_order.end()) { global.m_draw_order.erase(m_draw_order_it); }
}

TPtr<Camera> Camera::main() {
    for (auto &c : global.m_cameras) {
        if (c->enabled && c->tag.get() == "MainCamera")
            return c->shared_from_this();
    }

    return {};
}

std::vector<TPtr<Camera>> Camera::allCameras() {
    std::vector<TPtr<Camera>> result;
    result.reserve(global.m_cameras.size());

    for (auto &c : global.m_cameras) {
        if (c->enabled)
            result.emplace_back(c->shared_from_this());
    }
    return result;
}

void Camera::Render() {
    if (pixelRect == sf::FloatRect{0.f, 0.f, 0.f, 0.f}) {
        global.m_target.setView(sf::View({0.f, 0.f,
                                          static_cast<float>(global.m_target.getSize().x),
                                          static_cast<float>(global.m_target.getSize().y)}));
    } else {
        global.m_target.setView(sf::View(pixelRect));
    }

    ///global.m_target.draw()

}
