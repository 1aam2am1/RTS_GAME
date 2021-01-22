//
// Created by Michal_Marszalek on 2021-01-22.
//

#include <Core/MonoBehaviour.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Camera.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Core/Time.h>
#include <GameClient/Unity/Core/SpriteRenderer.h>

class SmartCamera : public MonoBehaviour {
public:
    TPtr<Camera> camera;
    TPtr<SpriteRenderer> block;

    void Start() override {
        auto vec = GetComponents<Camera>();
        for (auto &&v : vec) {
            if (v && v->name.empty()) {
                camera = v;
                break;
            }
        }
    }

    float x = 0;
    float y = 0;
    float speed = 5;
    float zakres = 60;

    ImVec2 size;

    void OnGUI() override {
        auto mouse = ImGui::GetMousePos();
        const auto window = ImGui::GetCurrentWindow();
        size = window->ContentRegionRect.GetSize();

        x = 0;
        y = 0;
        if (!ImGui::IsWindowHovered(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
            return;
        }

        mouse.x -= window->ContentRegionRect.Min.x;
        mouse.y -= window->ContentRegionRect.Min.y;

        if (mouse.x < zakres) {
            x = -speed * Time::deltaTime();
        } else if ((size.x - mouse.x) < zakres) {
            x = speed * Time::deltaTime();
        }

        if (mouse.y < zakres) {
            y = speed * Time::deltaTime();
        } else if ((size.y - mouse.y) < zakres) {
            y = -speed * Time::deltaTime();
        }
    }

    void LateUpdate() override {
        auto p = camera->transform()->localPosition();
        p.x += x;
        p.y += y;

        if (block && block->sprite) {
            const auto unit_size = block->sprite->s0.getTextureRect().getSize() / (int) block->sprite->pixelsPerUnit;
            const auto camera_size = sf::Vector2f{
                    static_cast<float>(2.f * camera->orthographicSize * (size.x / size.y)),
                    static_cast<float>(2.0f * camera->orthographicSize)};

            auto max_x = (unit_size.x - camera_size.x) / 2.0f;
            if (max_x > 0.f) {
                p.x = std::clamp(p.x, -max_x, max_x);

            } else {
                p.x = 0;
            }

            auto max_y = (unit_size.y - camera_size.y) / 2.0f;
            if (max_y > 0.f) {
                p.y = std::clamp(p.y, -max_y, max_y);

            } else {
                p.y = 0;
            }
        }

        camera->transform()->localPosition = p;
    }
};

ADD_USER_COMPONENT(SmartCamera, x, y, speed, zakres, block)
