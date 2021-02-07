//
// Created by Michal_Marszalek on 2021-01-19.
//

#include <Core/MonoBehaviour.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <Macro.h>
#include <GameClient/Unity/Editor/Menu.h>
#include <GameClient/Unity/Core/Application.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Components/Network/NetworkInterface.h>

class MenuPlayGame : public MonoBehaviour {
public:
    TPtr<NetworkInterface> network;

    void Start() override {
        network = FindObjectOfType<NetworkInterface>();
    }

    void OnGUI() override {
        ImGuiStyle &style = ImGui::GetStyle();
        const auto window = ImGui::GetCurrentWindow();
        const auto size = window->ContentRegionRect.GetSize();

        auto place_on_center = [&](auto str) {
            auto is = ImGui::CalcTextSize(str).x + style.ItemSpacing.x;
            ImGui::SetCursorPos({(size.x - is) / 2, ImGui::GetCursorPos().y + 20.f});
            return ImGui::Button(str);
        };

        ImGui::SetCursorPos({0, (size.y - 120.0f) / 2.0f});

        if (place_on_center("Play")) {
            ImGui::OpenPopup("###play");
        }
        if (place_on_center("Settings")) {
            SceneManager::LoadScene("Assets/menu/Settings.unity");
        }
        if (place_on_center("Exit")) {
            Application::Quit();
        }

        if (ImGui::BeginPopup("###play", ImGuiWindowFlags_AlwaysAutoResize)) {
            auto width = ImGui::CalcTextSize("Single").x + ImGui::GetStyle().FramePadding.x * 2.0f;

            ImGui::SetNextItemWidth(width);
            if (ImGui::Button("Single")) {
                SceneManager::LoadScene("Assets/menu/Play.unity");
            }

            ImGui::SetNextItemWidth(width);
            bool enabled_n = network.expired();
            if (enabled_n) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            }
            if (ImGui::Button("Multi")) {
                ImGui::OpenPopup("###multi");
            }
            if (enabled_n) {
                ImGui::PopItemFlag();
            }

            ImGui::SetNextItemWidth(width);
            if (ImGui::Button("Exit")) {
                ImGui::CloseCurrentPopup();
            }


            if (ImGui::BeginPopup("###multi", ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("IP:   ");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputText("##ip", &ip);

                ImGui::AlignTextToFramePadding();
                ImGui::Text("PORT: ");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(120);
                ImGui::InputInt("##port", &port, 0);

                if (ImGui::Button("Connect")) {
                    if (!network->Connect(ip, port)) {
                        ImGui::OpenPopup("###error");
                    } else {
                        DontDestroyOnLoad(network);
                        SceneManager::LoadScene("Assets/menu/Play.unity");
                    }
                }

                bool changed = network->IsOpenedPort();
                if (changed) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImColor(1.0f, 0.f, 0.f).Value);
                }
                if (ImGui::Button("Server")) {
                    if (!network->OpenPort(port)) {
                        ImGui::OpenPopup("###error");
                    }
                }
                if (changed) {
                    ImGui::PopStyleColor();
                    if (network->isConnected()) {
                        DontDestroyOnLoad(network);
                        SceneManager::LoadScene("Assets/menu/Play.unity");
                    }
                }


                if (ImGui::Button("Exit")) {
                    network->ClosePort();
                }

                if (ImGui::BeginPopup("###error", ImGuiWindowFlags_AlwaysAutoResize)) {
                    ImGui::NewLine();
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Something went wrong, try second time.");
                    ImGui::Text(" ");

                    ImGui::EndPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::EndPopup();
        }
    }

    std::string ip = "127.0.0.1";
    int port = 22653;
};

ADD_USER_COMPONENT(MenuPlayGame)
