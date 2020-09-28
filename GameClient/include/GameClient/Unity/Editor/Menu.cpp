//
// Created by Michal_Marszalek on 24.09.2020.
//

#include "Menu.h"
#include <imgui.h>
#include <utility>
#include <variant>

MENU_TAB("File", -25)
MENU_TAB("Edit", -21)
MENU_TAB("Assets", -17)
MENU_TAB("GameObject", -13)
MENU_TAB("Component", -9)
MENU_TAB("Window", -5)
MENU_TAB("Help", -1)

struct MenuItem {
    std::string name;
    int priority;

    struct Func {
        Func() = default;

        Func(const std::function<bool(MenuCommand)> &f, bool v) { add(f, v); }

        void add(std::function<bool(MenuCommand)> f, bool v) {
            if (v)validation.emplace_back(std::move(f));
            else execution.emplace_back(std::move(f));
        }

        void operator()(const MenuCommand &a) const {
            for (auto &v : validation) {
                if (!v(a)) { return; }
            }
            for (auto &e:execution) {
                e(a);
            }
        }

        std::vector<std::function<bool(MenuCommand)>> validation;
        std::vector<std::function<bool(MenuCommand)>> execution;
    };

    std::variant<Func, std::vector<MenuItem>> sub_items_function;
};

MenuItem &get_node(std::vector<MenuItem> &vector, std::string path, const int priority, const bool function) {
    auto last = path.empty() ? 0 : path.back() == '/';
    auto name = GameApi::substr(path, '/');

    auto create_node = [](std::vector<MenuItem> &vector, std::string name, const int priority,
                          const bool function) -> MenuItem & {
        auto it = std::find_if(vector.begin(), vector.end(), [&, same_name = false](const auto &l) mutable {
            if (l.priority > priority) { return true; }
            if (function && l.priority == priority && l.name == name && l.sub_items_function.index() == 1) {
                return true;
            }
            if (l.priority == priority && l.name == name) {
                same_name = true;
                return false;
            }
            if (same_name && l.name != name) { return true; }
            return false;
        });

        if (function) {
            return *vector.emplace(it, MenuItem{name, priority, MenuItem::Func{}});
        } else {
            return *vector.emplace(it, MenuItem{name, priority, std::vector<MenuItem>()});
        }
    };

    if (!path.empty() || last) {
        auto it = std::find_if(vector.begin(), vector.end(),
                               [&](const auto &l) { return l.name == name && l.sub_items_function.index() == 1; });
        if (it != vector.end()) {
            return get_node(std::get<1>(it->sub_items_function), path, priority, function);
        }

        auto &node = create_node(vector, name, priority, false);

        return get_node(std::get<1>(node.sub_items_function),
                        path, priority, function);
    }

    decltype(vector.begin()) it;
    if (function) {
        it = std::find_if(vector.begin(), vector.end(),
                          [&](const auto &l) {
                              return l.name == name && l.sub_items_function.index() == 0;
                          });
    } else {
        it = std::find_if(vector.begin(), vector.end(),
                          [&](const auto &l) {
                              return l.name == name && l.sub_items_function.index() == 1;
                          });
    }

    if (it != vector.end()) {
        if (it->priority != priority && (priority != 0 || !function)) {
            auto p = *it;
            vector.erase(it);
            auto &node = create_node(vector, name, priority, function);
            node.sub_items_function = p.sub_items_function;
            return node;
        }
        return *it;
    }

    return create_node(vector, name, priority, function);
}

std::vector<MenuItem> &global_item_vector() {
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
                    ImGui::Text("val: %i; exec: %i", p.validation.size(), p.execution.size());
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
