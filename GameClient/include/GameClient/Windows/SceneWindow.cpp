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
    ImVec2 pos;
    ImDrawList *list;

    DebugDraw(ImVec2 p, ImDrawList *l) : pos(p), list(l) {}

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        std::vector<ImVec2> points;
        points.reserve(vertexCount);

        for (int32_t i = 0; i < vertexCount; ++i) {
            points.emplace_back(vertices[i].x + pos.x, vertices[i].y + pos.y);
        }

        ImColor c(color.r, color.g, color.b, color.a);

        list->AddPolyline(points.data(), points.size(), c, true, 1.f);
    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        std::vector<ImVec2> points;
        points.reserve(vertexCount);

        for (int32_t i = 0; i < vertexCount; ++i) {
            points.emplace_back(vertices[i].x + pos.x, vertices[i].y + pos.y);
        }

        ImColor c(color.r, color.g, color.b, 60.f / 255.f);

        list->AddConvexPolyFilled(points.data(), points.size(), c);
        c.Value.w = 1.f;
        list->AddPolyline(points.data(), points.size(), c, true, 1.f);
    }

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override {
        ImVec2 position{pos.x + center.x, pos.y + center.y};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddCircle(position, radius, c);
    }

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override {
        ImVec2 position{pos.x + center.x, pos.y + center.y};
        ImColor c(color.r, color.g, color.b, 60.f / 255.f);
        ImVec2 endPoint{pos.x + center.x + radius * axis.x, pos.y + center.y + radius * axis.y};

        list->AddCircleFilled(position, radius, c);
        c.Value.w = 1.f;
        list->AddLine(position, endPoint, c);
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {
        ImVec2 position1{pos.x + p1.x, pos.y + p1.y};
        ImVec2 position2{pos.x + p2.x, pos.y + p2.y};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddLine(position1, position2, c);
    }

    void DrawTransform(const b2Transform &xf) override {
        const float k_axisScale = 0.4f;
        ImColor red(1.0f, 0.0f, 0.0f);
        ImColor green(0.0f, 1.0f, 0.0f);
        ImVec2 p1{xf.p.x + pos.x, xf.p.y + pos.y};
        ImVec2 p2;

        p2.x = p1.x + k_axisScale * xf.q.GetXAxis().x;
        p2.y = p1.y + k_axisScale * xf.q.GetXAxis().y;
        list->AddLine(p1, p2, red);

        p2.x = p1.x + k_axisScale * xf.q.GetYAxis().x;
        p2.y = p1.y + k_axisScale * xf.q.GetYAxis().y;
        list->AddLine(p1, p2, green);
    }

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override {
        ImVec2 position{pos.x + p.x, pos.y + p.y};
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
        window->flags |= ImGuiWindowFlags_MenuBar;
        window->Show();
    }

    void OnGUI() override {
        ///TODO: Has it own camera implementation
#if UNITY_EDITOR
        DrawGameSceneGizmoMenu();
        auto pos = ImGui::GetCursorScreenPos();

        ImDrawList *draw = ImGui::GetWindowDrawList();
        draw->ChannelsSplit(2);

        draw->ChannelsSetCurrent(0);
        ImGui::Image(global.m_target().getTexture());

        draw->ChannelsSetCurrent(1);
        if (global.mis.draw_gizmo) {
            DebugDraw d{pos, draw};
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
