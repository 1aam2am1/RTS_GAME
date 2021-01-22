//
// Created by Michal_Marszalek on 08.12.2020.
//

#include "SceneWindow.h"
#include "Macro.h"
#include <GameClient/GlobalStaticVariables.h>
#include <box2d/b2_draw.h>
#include <imgui.h>
#include <imgui_internal.h>


MENU_ITEM(SceneWindow::Init, "Window/General/Scene", 5);

#if UNITY_EDITOR
INITIALIZE_FUNC(SceneWindow::Init());
#endif

struct DebugDraw : b2Draw {
    sf::Transform transform;
    ImDrawList *list;
    int i = 1;
    const ImVec2 size;
    const ImVec2 cursor_pos;

    DebugDraw(sf::Transform t) : transform(t), size(ImGui::GetCurrentWindow()->ContentRegionRect.GetSize()),
                                 cursor_pos(ImGui::GetCursorPos()) {
        ImGui::SetItemAllowOverlap();
        ImGui::BeginChild(i, size, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
        list = ImGui::GetWindowDrawList();
    }

    ~DebugDraw() {
        ImGui::EndChild();
    }

    void Check() {
        if (list->_VtxCurrentIdx >= (1 << 15)) {
            ImGui::EndChild();
            ImGui::SetCursorPos(cursor_pos);
            ++i;
            ImGui::SetItemAllowOverlap();
            ImGui::BeginChild(i, size, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
            list = ImGui::GetWindowDrawList();
        }
    }

//TODO: Clip
    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {
        std::vector<ImVec2> points;
        points.reserve(vertexCount);

        for (int32_t i = 0; i < vertexCount; ++i) {

            points.emplace_back(transform.transformPoint(vertices[i].x, vertices[i].y));
        }

        ImColor c(color.r, color.g, color.b, color.a);

        list->AddPolyline(points.data(), points.size(), c, true, 1.f);

        Check();
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

        Check();
    }

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(center.x, center.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        auto p = transform.transformPoint(radius, 0) - transform.transformPoint(0, 0);
        radius = std::sqrt(p.x * p.x + p.y * p.y);
        list->AddCircle(position, radius, c);

        Check();
    }

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(center.x, center.y)};
        ImColor c(color.r, color.g, color.b, 60.f / 255.f);
        ImVec2 endPoint{transform.transformPoint(center.x + radius * axis.x, center.y + radius * axis.y)};

        auto p = transform.transformPoint(radius, 0) - transform.transformPoint(0, 0);
        radius = std::sqrt(p.x * p.x + p.y * p.y);
        list->AddCircleFilled(position, radius, c);
        c.Value.w = 1.f;
        list->AddLine(position, endPoint, c);

        Check();
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {
        ImVec2 position1{transform.transformPoint(p1.x, p1.y)};
        ImVec2 position2{transform.transformPoint(p2.x, p2.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddLine(position1, position2, c);

        Check();
    }

    void DrawTransform(const b2Transform &xf) override {
        const float k_axisScale = 0.4;

        ImColor red(1.0f, 0.0f, 0.0f);
        ImColor green(0.0f, 1.0f, 0.0f);
        ImVec2 p1{transform.transformPoint(xf.p.x, xf.p.y)};
        ImVec2 p2;

        p2 = transform.transformPoint(xf.p.x + k_axisScale * xf.q.GetXAxis().x,
                                      xf.p.y + k_axisScale * xf.q.GetXAxis().y);
        list->AddLine(p1, p2, red);

        p2 = transform.transformPoint(xf.p.x + k_axisScale * xf.q.GetYAxis().x,
                                      xf.p.y + k_axisScale * xf.q.GetYAxis().y);
        list->AddLine(p1, p2, green);

        Check();
    }

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override {
        ImVec2 position{transform.transformPoint(p.x, p.y)};
        ImColor c(color.r, color.g, color.b, color.a);

        list->AddCircleFilled(position, size, c);

        Check();
    }
};


#if UNITY_EDITOR

void SceneWindow::Awake() {
    orthographicSize = global.settings.old_orthographicSize;
}

void SceneWindow::DrawCamera() {
    //Code from Camera
    const ImGuiWindow *window = ImGui::GetCurrentWindow();

    sf::Vector2f size{window->ContentRegionRect.GetWidth(), window->ContentRegionRect.GetHeight()};
    texture.create(size.x, size.y);
    texture.clear(sf::Color::Black);

    {
        for (auto x = 0; x < size.x / 10; ++x) {
            for (auto y = 0; y < size.y / 10; ++y) {

            }
        }
    }

    if (size.y != 0) {
        size.x = 2.f * orthographicSize * (size.x / size.y);
        size.y = 2.f * orthographicSize;
    } else {
        return;
    }

    sf::View view({position.x, -position.y}, size);

    texture.setView(view);

    for (auto &it : global.rendering.m_render) {
        if (it && it->m_onEnable && !it->forceRenderingOff)
            it->draw(&texture);
    }

    texture.display();
}

void SceneWindow::OnStyleChange() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void SceneWindow::OnStylePop() {
    ImGui::PopStyleVar();
}

#endif

void SceneWindow::OnGUI() {
    ///TODO: Has it own camera implementation
#if UNITY_EDITOR
    DrawGameSceneGizmoMenu();
    const auto pos = ImGui::GetCursorScreenPos();
    const auto window = ImGui::GetCurrentWindow();
    const auto size = window->ContentRegionRect.GetSize();
    const ImVec2 origin = {pos.x + size.x / 2.f, pos.y + size.y / 2.f};
    const auto cursor_pos = ImGui::GetCursorPos();

    /** Drawing */
    DrawCamera();
    ImGui::SetItemAllowOverlap();
    ImGui::Image(texture.getTexture());
    ImGui::SetCursorPos(cursor_pos);

    if (global.mis.draw_gizmo) {
        sf::Transform transform;

        transform.translate(origin.x, origin.y);
        transform.scale(texture.getSize().y / (2.f * orthographicSize),
                        texture.getSize().y / (2.f * orthographicSize));

        transform.translate(-position.x, position.y);

        transform.scale(1.f, -1.f);

        //draw->AddCircle({pos.x+size.x, pos.y+size.y},5, ImColor(1.f,0.f,0.f));

        DebugDraw d{transform};
        d.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/ | b2Draw::e_centerOfMassBit);

        global.physics.world.SetDebugDraw(&d);

        global.physics.world.DebugDraw();

        global.physics.world.SetDebugDraw(nullptr);
    }

    ImGui::SetCursorPos(cursor_pos);
    /** Move/Select/... */
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::SetItemAllowOverlap();
        ImGui::BeginChild("###hehe", size);

        ImGui::InvisibleButton("canvas", size,
                               ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered(); // Hovered
        const bool is_active = ImGui::IsItemActive();   // Held

        if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            //Select body
        }

        if (is_hovered) {
            auto delta = io.MouseWheel * 0.2f * orthographicSize;
            orthographicSize += delta;
            if (orthographicSize <= 0.01f) { orthographicSize = 0.01f; }

            ImVec2 from_origin = {io.MousePos.x - origin.x, io.MousePos.y - origin.y};

            position.x -= from_origin.x * 2.0f * (delta / window->ContentRegionRect.GetHeight());
            position.y += from_origin.y * 2.0f * (delta / window->ContentRegionRect.GetHeight());

        }

        if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
            position.x -= io.MouseDelta.x * (2.f * orthographicSize) / window->ContentRegionRect.GetHeight();
            position.y += io.MouseDelta.y * (2.f * orthographicSize) / window->ContentRegionRect.GetHeight();

            ImGui::SetMouseCursor(7);
        }

        /*
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
            ImGui::OpenPopupOnItemClick("context");*/

        ImGui::EndChild();
    }
#endif
}

