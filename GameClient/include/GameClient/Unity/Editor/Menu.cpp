//
// Created by Michal_Marszalek on 24.09.2020.
//

#include "Menu.h"
#include <imgui.h>
#include <utility>
#include <variant>
#include <GameClient/Unity/Editor/MenuItem.h>

MENU_TAB("File", -25)
MENU_TAB("Edit", -21)
MENU_TAB("Assets", -17)
MENU_TAB("GameObject", -13)
MENU_TAB("Component", -9)
MENU_TAB("Window", -5)
MENU_TAB("Window/General", 30)
MENU_TAB("Help", -1)


static std::vector<MenuItem> &global_item_vector() {
    static std::vector<MenuItem> items;
    return items;
}

Menu::Menu() : items(global_item_vector()) {}

void Menu::drawGui() {
    if (work != NotShown && ImGui::BeginMainMenuBar()) {
        OnGUI();
        ImGui::EndMainMenuBar();
    }
}

void Menu::OnGUI() {
    std::function<void(std::vector<MenuItem> &)> display = [&](std::vector<MenuItem> &vector) {
        int priority = 0;
        if (!vector.empty()) { priority = vector.front().priority; }
        for (auto &i : vector) {
            if (i.priority - priority > 10) {
                ImGui::Separator();
            }
            priority = i.priority;
            if (i.sub_items_function.index() == 0) {
                if (ImGui::MenuItem(i.name.c_str())) {
                    const auto &f = std::get<0>(i.sub_items_function);
                    f({});
                }
            } else if (i.sub_items_function.index() == 1) {
                if (std::get<1>(i.sub_items_function).empty()) {
                    ImGui::MenuItem(i.name.c_str());
                } else if (ImGui::BeginMenu(i.name.c_str())) {
                    display(std::get<1>(i.sub_items_function));
                    ImGui::EndMenu();
                }
            }//else should throw but the implementation should be correct one and therefore this branch don't exists.
        }
    };
    display(items);
}


void Menu::addItem(std::string path, std::function<bool(MenuCommand)> f, bool validation, int priority) {
    std::get<0>(get_node(global_item_vector(), std::move(path), priority, true).sub_items_function).add(
            std::move(f), validation);
}

void Menu::addPlaceHolder(std::string path, int priority) {
    get_node(global_item_vector(), std::move(path), priority, false);
}

/// Debug display of items
class HELP : public EditorWindow {
public:
    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<HELP>();
        window->ShowUtility();
    }

    void OnGUI() override {
        ImGui::Text("Focused Window: %s", focusedWindow() ? focusedWindow()->titleContent.c_str() : "");
        ImGui::Text("Hovered Window: %s", mouseOverWindow() ? mouseOverWindow()->titleContent.c_str() : "");
        ImGui::Separator();
        {
            auto p = position();
            ImGui::Text("Position x: %.2f y: %.2f", p.getPosition().x, p.getPosition().y);
            ImGui::Text("Size     x: %.2f y: %.2f", p.getSize().x, p.getSize().y);
        }
        ImGui::Separator();
        if (ImGui::Button("Set size")) {
            auto p = position();
            p.left = 100.f;
            position = p;
        }
        ImGui::Separator();
        display(global_item_vector());
    }

    void display(std::vector<MenuItem> items) {
        for (auto &i : items) {
            if (ImGui::TreeNode(i.name.c_str(), "%s: %i", i.name.c_str(), i.priority)) {
                if (i.sub_items_function.index() == 0) {
                    const auto &p = std::get<0>(i.sub_items_function);
                    ImGui::Text("val: %u; exec: %u", static_cast<uint32_t>(p.validation.size()),
                                static_cast<uint32_t>(p.execution.size()));
                } else if (i.sub_items_function.index() == 1) {
                    display(std::get<1>(i.sub_items_function));
                } else {
                    std::terminate();
                }

                ImGui::TreePop();
            }
        }
    }
};

MENU_ITEM(HELP::Init, "Help/HELP")
