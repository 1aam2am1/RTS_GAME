//
// Created by Michal_Marszalek on 19.11.2020.
//


#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/WindowLayout.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/SceneManagement/Scene.h>
#include <imgui.h>
#include <GameClient/TPtr.h>

class SceneWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<SceneWindow>();
        WindowLayout::dockWindow(WindowLayout::Left, window);
        window->Show();
    }

    void OnGUI() override {
        for (auto &it : SceneManager::data) {
            if (!it.second.isValid) { continue; }
            if (!it.second.isLoaded) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }

            ImGui::PushID(it.first);
            //print scene name
            bool open = ImGui::TreeNodeEx(it.second.name.data(),
                                          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth |
                                          ImGuiTreeNodeFlags_DefaultOpen);

            if (open) {
                std::function<void(const TPtr<GameObject> &)> printAll = [&printAll](const TPtr<GameObject> &d) {
                    if (!d) { return; }

                    //print game object
                    if (!d->activeSelf()) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    }
                    bool open = ImGui::TreeNodeEx(d->name.data(),
                                                  ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth);
                    if (!d->activeSelf()) {
                        ImGui::PopStyleVar();
                    }

                    for (int i = 0; open && i < d->transform()->childCount(); ++i) {
                        printAll(d->transform()->GetChild(i)->gameObject());
                    }

                    if (open) { ImGui::TreePop(); }
                };
                //for all child game objects
                for (auto &it2 : it.second.objects) {
                    printAll(it2);
                }

                ImGui::TreePop();
            }
            ImGui::PopID();

            if (!it.second.isLoaded) {
                ImGui::PopStyleVar();
            }
        }
    }
};

MENU_ITEM(SceneWindow::Init, "Window/General/Scene", 3);

#if UNITY_EDITOR
INITIALIZE_FUNC(SceneWindow::Init());
#endif
