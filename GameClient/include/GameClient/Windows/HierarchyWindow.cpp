//
// Created by Michal_Marszalek on 19.11.2020.
//


#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/WindowLayout.h>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <imgui.h>

class HierarchyWindow : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<HierarchyWindow>();
        WindowLayout::dockWindow(WindowLayout::Left, window);
        window->Show();
    }

    void Awake() override {
        titleContent = "Hierarchy";
    }

    void OnGUI() override {
        bool clear_selection = true;

        for (auto &it : SceneManager::data) {
            bool isLoaded = it.second.isLoaded;
            if (!isLoaded) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            }

            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth |
                                            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            if (it.first == SceneManager::active_scene) {
                node_flags |= ImGuiTreeNodeFlags_Selected;
            }

            ImGui::PushID(it.first);
            //print scene name
            bool open = ImGui::TreeNodeEx(it.second.name.data(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsMouseDoubleClicked(0)) {
                SceneManager::active_scene = it.first;
                clear_selection = false;
            } else if (ImGui::IsItemClicked(1)) {
                Selection::activeGameObject = nullptr;
                Selection::activeObject = nullptr;
                Selection::activeTransform = nullptr;
                Selection::assetGUIDs.clear();
                ContextMenu::Init("GameObject");
            }

            if (open) {
                std::function<void(const TPtr<GameObject> &)> printAll = [&printAll, &clear_selection](
                        const TPtr<GameObject> &d) {
                    if (!d) { return; }
                    bool activeSelf = d->activeSelf();

                    //print game object
                    if (!activeSelf) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    }
                    std::string key = d->name + "###" + GameApi::to_string(d.get());
                    bool open;
                    ImGuiTreeNodeFlags node_flags =
                            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                            ImGuiTreeNodeFlags_SpanAvailWidth;

                    if (!d->transform()->childCount()) {
                        node_flags |= ImGuiTreeNodeFlags_Leaf;
                    }
                    if (Selection::activeGameObject == d) {
                        node_flags |= ImGuiTreeNodeFlags_Selected;
                    }

                    open = ImGui::TreeNodeEx(key.data(), node_flags);
                    if ((ImGui::IsItemClicked(1) || ImGui::IsItemClicked()) && !ImGui::IsMouseDoubleClicked(0)) {
                        Selection::activeGameObject = d;
                        Selection::activeObject = d;
                        Selection::activeTransform = d->transform();
                        Selection::assetGUIDs.clear();
                        clear_selection = false;

                        if (ImGui::IsItemClicked(1)) {
                            ContextMenu::Init("GameObject");
                        }
                    }

                    if (!activeSelf) {
                        ImGui::PopStyleColor();
                    }

                    for (int i = 0; open && i < d->transform()->childCount(); ++i) {
                        printAll(d->transform()->GetChild(i)->gameObject());
                    }

                    if (open) { ImGui::TreePop(); }
                };
                //for all child game objects
                for (auto &it2 : it.second.root) {
                    printAll(it2);
                }

                ImGui::TreePop();
            }
            ImGui::PopID();

            if (!isLoaded) {
                ImGui::PopStyleColor();
            }
        }

        if (clear_selection && ImGui::IsWindowHovered() && ImGui::IsMouseDoubleClicked(0)) {
            Selection::activeGameObject = nullptr;
            Selection::activeObject = nullptr;
            Selection::activeTransform = nullptr;
            Selection::assetGUIDs.clear();
        }
    }
};

MENU_ITEM(HierarchyWindow::Init, "Window/General/Scene", 3);

#if UNITY_EDITOR
INITIALIZE_FUNC(HierarchyWindow::Init());
#endif
