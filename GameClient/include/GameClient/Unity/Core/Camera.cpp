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
#include <GameClient/Unity/Serialization/to_json.h>

ADD_COMPONENT_MENU(Camera, depth, backgroundColor, orthographicSize, pixelRect)
ADD_ATTRIBUTE(Camera, ExecuteInEditMode)

Camera::Camera() : depth(
        [&](auto d) {
            auto it = std::find_if(std::execution::par_unseq, global.rendering.m_draw_order.begin(),
                                   global.rendering.m_draw_order.end(),
                                   [this](auto &it) { return it.second.get() == this; });
            if (it != global.rendering.m_draw_order.end()) { global.rendering.m_draw_order.erase(it); }

            m_depth = d;
            global.rendering.m_draw_order.emplace(m_depth, shared_from_this());

        }, [&]() { return m_depth; }) {
}

Camera::~Camera() {

}

void Camera::Awake() {
    global.rendering.m_cameras.emplace_back(shared_from_this());
    global.rendering.m_draw_order.emplace(m_depth, shared_from_this());
}

TPtr<Camera> Camera::main() {
    for (auto &c : global.rendering.m_cameras) {
        if (c->enabled && c->tag.get() == "MainCamera")
            return c->shared_from_this();
    }

    return {};
}

std::vector<TPtr<Camera>> Camera::allCameras() {
    std::vector<TPtr<Camera>> result;
    result.reserve(global.rendering.m_cameras.size());

    for (auto &c : global.rendering.m_cameras) {
        if (c->enabled)
            result.emplace_back(c->shared_from_this());
    }
    return result;
}

void Camera::Render() {
    auto position = transform()->localPosition.get();
    sf::Vector2f size{global.rendering.m_target().getSize()};
    if (size.y != 0 && pixelRect.getSize().x != 0 && pixelRect.getSize().x != 0) {
        size.x = 2.f * orthographicSize * (size.x / size.y) * pixelRect.getSize().x;
        size.y = 2.f * orthographicSize * pixelRect.getSize().y;
    } else {
        return;
    }

    sf::View view({position.x, -position.y}, size);
    view.setViewport(pixelRect);
    view.setRotation(transform()->localRotation);

    global.rendering.m_target().setView(view);

    global.rendering.m_target().clear(backgroundColor);//TODO: Clear only pixelRect

    for (auto &it : global.rendering.m_render) {
        if (it && it->m_onEnable && !it->forceRenderingOff)
            it->draw();
    }


    ///glDisable(GL_SCISSOR_TEST);

#if UNITY_EDITOR
    global.rendering.m_target().display(); //Display only in editor as window is displayed in main screen
#endif
}

void Camera::OnEnable() {

}

void Camera::OnDisable() {

}
