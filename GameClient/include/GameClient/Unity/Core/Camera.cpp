//
// Created by Michal_Marszalek on 14.12.2020.
//

#include "Camera.h"
#include <GameClient/GlobalStaticVariables.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <execution>
#include <GameClient/Unity/Core/Attributes.h>
#include <GameClient/Unity/Serialization/to_json.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Renderer.h"

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

    {
        global.rendering.m_target().setView(global.rendering.m_target().getDefaultView());
        auto s = pixelRect.getSize();
        s.x = std::clamp(s.x, 0.f, 1.f);
        s.y = std::clamp(s.y, 0.f, 0.f);
        sf::RectangleShape shape({global.rendering.m_target().getSize().x * s.x,
                                  global.rendering.m_target().getSize().y * s.y});

        shape.setPosition(global.rendering.m_target().getSize().x * pixelRect.left,
                          global.rendering.m_target().getSize().y * pixelRect.top);

        shape.setFillColor(backgroundColor);
        global.rendering.m_target().draw(shape, sf::BlendNone);
        //global.rendering.m_target().clear(backgroundColor);//TODO: Clear only pixelRect
    }

    sf::View view({position.x, -position.y}, size);
    view.setViewport(pixelRect);
    view.setRotation(transform()->localRotation);

    global.rendering.m_target().setView(view);

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

void Camera::Update() {

}
