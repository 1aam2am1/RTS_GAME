//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <imgui.h>
#include <Core/MonoBehaviour.h>
#include <Macro.h>
#include <imgui_internal.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/GlobalStaticVariables.h>
#include <SFML/Window.hpp>
#include <Audio/AudioListener.h>

class SettingsGameWindow : public MonoBehaviour {
public:
    bool fullscreen = false;
    sf::Vector2u size = {1200, 800};
    bool auto_size = false;
    uint32_t antialiasing = 0;

    float old_volume = 0;
    sf::Clock clock;

    void Start() override {
        RefreshAttributes();
    }

    void RefreshAttributes() {
        fullscreen = global.settings.window.fullscreen;
        global.settings.window.size = size = global.rendering.m_window->getSize();
        auto_size = global.settings.window.auto_size;
        antialiasing = global.rendering.m_window->getSettings().antialiasingLevel;

        old_volume = AudioListener::volume;
    }

    void ResetAttributes() {
        AudioListener::volume = old_volume;


        RefreshAttributes();
    }

    void OnGUI() override {
        const auto window = ImGui::GetCurrentWindow();
        const auto ss = window->ContentRegionRect.GetSize();
        bool modal = false;

        ImVec2 c_size{350, 200};
        ImGui::SetCursorPos({(ss.x - c_size.x) / 2.0f, (ss.y - c_size.y) / 2.0f});
        if (ImGui::BeginChild("##settings", c_size, false,
                              ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration)) {
            ImGui::Text("Settings");

            if (ImGui::BeginTable("##s", 2)) {
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 1);

                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Antialiasing");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-FLT_MIN);
                if (ImGui::BeginCombo("##antialiasingLevel", GameApi::to_string(antialiasing).data())) {
                    for (auto &&s : {0u, 1u, 2u, 4u, 8u}) {
                        const bool is_selected = (s == antialiasing);
                        if (ImGui::Selectable(GameApi::to_string(s).data(), is_selected))
                            antialiasing = s;

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                //antyaliasing levels
                ImGui::TableNextColumn();

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Fullscreen");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::Checkbox("##full", &fullscreen);
                ImGui::TableNextColumn();

                if (!fullscreen) {
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Auto resolution");
                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::Checkbox("##auto", &auto_size);
                    ImGui::TableNextColumn();
                }

                if (!auto_size || fullscreen) {
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Resolution");
                    ImGui::TableNextColumn();
                    auto preview = "{ " + GameApi::to_string(size.x) + " X " + GameApi::to_string(size.y) + "}";

                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (ImGui::BeginCombo("##resulution", preview.data())) {
                        if (fullscreen) {
                            auto l = [](const sf::Vector2u &l, const sf::Vector2u &r) {
                                if (l.x < r.x) { return true; }
                                if (l.x == r.x) { return l.y < r.y; }
                                return false;
                            };
                            static const std::set<sf::Vector2u, decltype(l)> resolution = [l]() {
                                std::set<sf::Vector2u, decltype(l)> result(l);
                                auto modes = sf::VideoMode::getFullscreenModes();

                                for (auto &&m : modes) {
                                    result.emplace(m.width, m.height);
                                }

                                return result;
                            }();

                            for (auto &&m : resolution) {
                                const bool is_selected = (m.x == size.x && m.y == size.y);
                                auto p =
                                        "{ " + GameApi::to_string(m.x) + " X " + GameApi::to_string(m.y) + "}";
                                if (ImGui::Selectable(p.data(), is_selected)) {
                                    size.x = m.x;
                                    size.y = m.y;
                                }

                                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                        } else {
                            static const std::array<sf::Vector2u, 14> resolution = {{{800, 600},
                                                                                            {800, 600},
                                                                                            {1024, 600},
                                                                                            {1024, 768},
                                                                                            {1280, 720},
                                                                                            {1280, 1024},
                                                                                            {1366, 768},
                                                                                            {1400, 1050},
                                                                                            {1440, 900},
                                                                                            {1600, 900},
                                                                                            {1600, 1024},
                                                                                            {1600, 1200},
                                                                                            {1680, 1050},
                                                                                            {1920, 1080}}};

                            for (auto &&m : resolution) {
                                const bool is_selected = (m.x == size.x && m.y == size.y);
                                auto p =
                                        "{ " + GameApi::to_string(m.x) + " X " + GameApi::to_string(m.y) + "}";
                                if (ImGui::Selectable(p.data(), is_selected)) {
                                    size.x = m.x;
                                    size.y = m.y;
                                }

                                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                        }

                        ImGui::EndCombo();
                    }
                    ImGui::TableNextColumn();
                }

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Volume");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-FLT_MIN);
                float v = AudioListener::volume;
                if (ImGui::SliderFloat("##vol", &v, 0, 1, "%.2f")) {
                    AudioListener::volume = v;
                }
                //volume levels
                ImGui::TableNextColumn();

                ImGui::TableNextColumn();//Empty row
                ImGui::AlignTextToFramePadding();
                ImGui::Text(" ");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-FLT_MIN);
                if (ImGui::Button("Apply")) {
                    modal = true;
                    auto style = sf::Style::Titlebar | sf::Style::Close;
                    if (fullscreen) {
                        style |= sf::Style::Fullscreen;
                    }
                    if (auto_size) {
                        style |= sf::Style::Resize;
                    }
                    sf::ContextSettings context{0, 0, 0, 2, 1};
                    context.antialiasingLevel = antialiasing;
                    global.rendering.m_window->create(sf::VideoMode{size.x, size.y}, "RTS_GAME",
                                                      style, context);
                    clock.restart();
                }
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(-FLT_MIN);
                if (ImGui::Button("Reset")) {
                    ResetAttributes();
                }
                ImGui::TableNextColumn();

                ImGui::TableNextColumn();//Empty row
                ImGui::AlignTextToFramePadding();
                ImGui::Text(" ");
                ImGui::TableNextColumn();

                if (ImGui::Button("Exit")) {
                    ///TODO: Load main screen
                }

                ImGui::EndTable();
            }
        }

        if (modal) {
            modal = false;
            ImGui::OpenPopup("###Aprove");
        }

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Apply changes or undo? ###Aprove", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

            ImGui::SetNextItemWidth(120);
            if (ImGui::Button("    Ok    ")) {
                global.settings.window.fullscreen = fullscreen;
                global.settings.window.size = size = global.rendering.m_window->getSize();
                global.settings.window.auto_size = auto_size;

                old_volume = AudioListener::volume;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);

            ImGui::SetNextItemWidth(120);
            if (ImGui::Button(("Undo: " + GameApi::to_string(30 - (int) clock.getElapsedTime().asSeconds()) +
                               "##return2").data()) || (clock.getElapsedTime().asSeconds() > 30)) {
                auto style = sf::Style::Titlebar | sf::Style::Close;
                if (global.settings.window.fullscreen) {
                    style |= sf::Style::Fullscreen;
                }
                if (global.settings.window.auto_size) {
                    style |= sf::Style::Resize;
                }
                sf::ContextSettings context{0, 0, 0, 2, 1};
                context.antialiasingLevel = global.settings.window.antialiasing;

                global.rendering.m_window->create(
                        sf::VideoMode{global.settings.window.size.x, global.settings.window.size.y}, "RTS_GAME",
                        style, context);

                ResetAttributes();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::EndChild();
    }
};

ADD_USER_COMPONENT(SettingsGameWindow)
