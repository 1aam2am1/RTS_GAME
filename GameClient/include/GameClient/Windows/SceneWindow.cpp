//
// Created by Michal_Marszalek on 08.12.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include <GameClient/WindowLayout.h>
#include "Macro.h"
#include <imgui.h>
#include <GameClient/GlobalStaticVariables.h>
#include <imgui-SFML.h>
#include <GameClient/Windows/GameSceneMenuGizmo.h>

struct DebugDraw : b2Draw {
    sf::Transform transform;
    ImDrawList *list;

    DebugDraw(sf::Transform t, ImDrawList *l) : transform(t), list(l) {}

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        std::vector<ImVec2> points;
        points.reserve(vertexCount);

        for (int32_t i = 0; i < vertexCount; ++i) {

            points.emplace_back(transform.transformPoint(vertices[i].x, vertices[i].y));
        }

        ImColor c(color.r, color.g, color.b, color.a);

        list->AddPolyline(points.data(), points.size(), c, true, 1.f);
    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        std::vector<ImVec2> points;
        points.reserve(vertexCount);

        for (int32_t i = 0; i < vertexCount; ++i) {
            points.emplace_back(transform.transformPoint(vertices[i].x, vertices[i].y));
        }

        ImColor c(color.r, color.g, color.b, 60.f / 255.f);

        list->AddConvexPolyFilled(points.data(), points.size(), c);
        c.Value.w = 1.f;
        list->AddPolyline(points.data(), points.size(), c, true, 1.f);
    }

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(center.x, center.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddCircle(position, radius, c);
    }

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(center.x, center.y)};
        ImColor c(color.r, color.g, color.b, 60.f / 255.f);
        ImVec2 endPoint{transform.transformPoint(center.x + radius * axis.x, center.y + radius * axis.y)};

        list->AddCircleFilled(position, radius, c);
        c.Value.w = 1.f;
        list->AddLine(position, endPoint, c);
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {
        ImVec2 position1{transform.transformPoint(p1.x, p1.y)};
        ImVec2 position2{transform.transformPoint(p2.x, p2.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddLine(position1, position2, c);
    }

    void DrawTransform(const b2Transform &xf) override {
        const float k_axisScale = [&]() {
            auto vec = transform.transformPoint(0.4, 0);
            return std::sqrt(vec.x * vec.x + vec.y * vec.y);
        }();

        ImColor red(1.0f, 0.0f, 0.0f);
        ImColor green(0.0f, 1.0f, 0.0f);
        ImVec2 p1{transform.transformPoint(xf.p.x, xf.p.y)};
        ImVec2 p2;

        p2.x = p1.x + k_axisScale * xf.q.GetXAxis().x;
        p2.y = p1.y + k_axisScale * xf.q.GetXAxis().y;
        list->AddLine(p1, p2, red);

        p2.x = p1.x + k_axisScale * xf.q.GetYAxis().x;
        p2.y = p1.y + k_axisScale * xf.q.GetYAxis().y;
        list->AddLine(p1, p2, green);
    }

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(p.x, p.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddCircleFilled(position, size, c);
    }
};

class SceneWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<SceneWindow>();
        WindowLayout::dockWindow(WindowLayout::Center, window);
        window->flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
        window->Show();
    }

#if UNITY_EDITOR
    sf::Vector2f position = {0, 0};
    float orthographicSize = 1; //Zoom
    sf::RenderTexture texture;

    void DrawCamera() {
        //Code from Camera
        ImGuiWindow *window = ImGui::GetCurrentWindow();

        sf::Vector2f size{window->ContentRegionRect.GetWidth(), window->ContentRegionRect.GetHeight()};
        texture.create(size.x, size.y);

        if (size.y != 0) {
            size.x = 2.f * orthographicSize * (size.x / size.y);
            size.y = 2.f * orthographicSize;
        } else {
            return;
        }

        sf::View view({position.x, -position.y}, size);

        texture.setView(view);

        texture.clear(sf::Color::Black);

        for (auto &it : global.m_render) {
            if (it && it->m_onEnable && !it->forceRenderingOff)
                it->draw(&texture);
        }

        texture.display();
    }

#endif

    void OnGUI() override {
        ///TODO: Has it own camera implementation
#if UNITY_EDITOR
        DrawGameSceneGizmoMenu();
        DrawCamera();
        auto pos = ImGui::GetCursorScreenPos();

        ImDrawList *draw = ImGui::GetWindowDrawList();
        draw->ChannelsSplit(2);

        draw->ChannelsSetCurrent(0);
        ImGui::Image(texture.getTexture());

        draw->ChannelsSetCurrent(1);
        if (global.mis.draw_gizmo) {
            sf::Transform transform;
            auto window = ImGui::GetCurrentWindow();
            auto size = window->ContentRegionRect.GetSize();
            size.x /= 2.f;
            size.y /= 2.f;

            transform.translate(pos.x, pos.y);
            transform.translate(size.x, size.y);
            transform.scale(texture.getSize().y / (2.f * orthographicSize),
                            texture.getSize().y / (2.f * orthographicSize));


            DebugDraw d{transform, draw};
            d.SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);

            global.physics.world.SetDebugDraw(&d);

            global.physics.world.DebugDraw();

            global.physics.world.SetDebugDraw(nullptr);
        }

        draw->ChannelsMerge();
#endif
    }

};


MENU_ITEM(SceneWindow::Init, "Window/General/Scene", 5);

#if UNITY_EDITOR
INITIALIZE_FUNC(SceneWindow::Init());
#endif
