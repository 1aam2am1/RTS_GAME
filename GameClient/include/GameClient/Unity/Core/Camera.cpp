//
// Created by Michal_Marszalek on 14.12.2020.
//

#include "Camera.h"
#include <GameClient/GlobalStaticVariables.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Editor/Selection.h>
#include <SFML/OpenGL.hpp>
#include <execution>
#include <GameClient/Unity/Core/Attributes.h>

ADD_COMPONENT_MENU(Camera, depth, backgroundColor, orthographicSize)
ADD_ATTRIBUTE(Camera, ExecuteInEditMode)

Camera::Camera() : depth(
        [&](auto d) {
            auto it = std::find_if(std::execution::par_unseq, global.m_draw_order.begin(), global.m_draw_order.end(),
                                   [this](auto &it) { return it.second.get() == this; });
            if (it != global.m_draw_order.end()) { global.m_draw_order.erase(it); }

            m_depth = d;
            global.m_draw_order.emplace(m_depth, shared_from_this());

        }, [&]() { return m_depth; }) {
}

Camera::~Camera() {

}

void Camera::Awake() {
    global.m_cameras.emplace_back(shared_from_this());
    global.m_draw_order.emplace(m_depth, shared_from_this());
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
    auto position = transform()->localPosition.get();
    //TODO: !!! Change to global position and rotation
    //TODO: !!! Check order of zoom/move/rotate
    /* view.zoom(transform()->localScale.get().z);
     view.move(position.x, position.y);
     view.setRotation(transform()->localRotation);

     global.m_target.setView(view);
     global.m_target.setActive();*/
/*
    glEnable(GL_SCISSOR_TEST);

    //TODO: !!! Check glScissor
    glScissor((int) pixelRect.left * global.m_target.getSize().x,
              (int) pixelRect.top * global.m_target.getSize().y,
              (int) pixelRect.width * global.m_target.getSize().x,
              (int) pixelRect.height * global.m_target.getSize().y);
*/
    ///global.m_target.draw()
//TODO: Pixel per unit
    global.m_target.clear(backgroundColor);

    global.m_target.setView(sf::View({0.f, 0.f}, sf::Vector2f{global.m_target.getSize()}));

    for (auto &it : global.m_render) {
        if (it && it->enabled && !it->forceRenderingOff && it->gameObject()->activeInHierarchy())
            it->draw();
    }


    ///glDisable(GL_SCISSOR_TEST);

    global.m_target.display();
}
