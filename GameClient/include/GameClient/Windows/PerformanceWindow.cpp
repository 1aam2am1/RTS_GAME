//
// Created by Michal_Marszalek on 2021-02-03.
//

#include <Macro.h>
#include <Editor/Menu.h>
#include <imgui.h>
#include <GameClient/Performance.h>

struct PerformanceWindow : public EditorWindow{
    static void Init() {
        // GetData existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<PerformanceWindow>();
        window->ShowUtility();
    }

    void OnGUI() override {
        ImGui::Text("Event:                 %lldus", performance.GetData(Performance::Event).count());
        ImGui::Text("Start:                 %lldus", performance.GetData(Performance::Start).count());
        ImGui::Text("FixedUpdate:           %lldus", performance.GetData(Performance::FixedUpdate).count());
        ImGui::Text("Box2dStep:             %lldus", performance.GetData(Performance::Box2dStep).count());
        ImGui::Text("OnTriggerEnter2D:      %lldus", performance.GetData(Performance::OnTriggerEnter2D).count());
        ImGui::Text("OnTriggerStay2D:       %lldus", performance.GetData(Performance::OnTriggerStay2D).count());
        ImGui::Text("OnTriggerExit2D:       %lldus", performance.GetData(Performance::OnTriggerExit2D).count());
        ImGui::Text("OnCollisionEnter2D:    %lldus", performance.GetData(Performance::OnCollisionEnter2D).count());
        ImGui::Text("OnCollisionStay2D:     %lldus", performance.GetData(Performance::OnCollisionStay2D).count());
        ImGui::Text("OnCollisionExit2D:     %lldus", performance.GetData(Performance::OnCollisionExit2D).count());
        ImGui::Text("ContactListenerUpdate: %lldus", performance.GetData(Performance::ContactListenerUpdate).count());
        ImGui::Text("Update:                %lldus", performance.GetData(Performance::Update).count());
        ImGui::Text("LateUpdate:            %lldus", performance.GetData(Performance::LateUpdate).count());
        ImGui::Text("BeforeDraw:            %lldus", performance.GetData(Performance::BeforeDraw).count());
        ImGui::Text("SceneRendering:        %lldus", performance.GetData(Performance::SceneRendering).count());
        ImGui::Text("WindowUpdate:          %lldus", performance.GetData(Performance::WindowUpdate).count());
        ImGui::Text("WindowDraw:            %lldus", performance.GetData(Performance::WindowDraw).count());
        ImGui::Text("Display:               %lldus", performance.GetData(Performance::Display).count());
        ImGui::Text("Invoke:                %lldus", performance.GetData(Performance::Invoke).count());
        ImGui::Text("RunInitializer:        %lldus", performance.GetData(Performance::RunInitializer).count());
        ImGui::Text("Synchronize:           %lldus", performance.GetData(Performance::Synchronize).count());
    }
};

MENU_ITEM(PerformanceWindow::Init, "Help/Performance", 3);
