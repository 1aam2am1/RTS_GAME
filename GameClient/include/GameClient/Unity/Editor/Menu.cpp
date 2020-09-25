//
// Created by Michal_Marszalek on 24.09.2020.
//

#include "Menu.h"
#include <imgui.h>
#include <variant>

MENU_TAB("File", 1)
MENU_TAB("Edit", 2)
MENU_TAB("Assets", 3)
MENU_TAB("GameObject", 4)
MENU_TAB("Component", 5)
MENU_TAB("Window", 6)
MENU_TAB("Help", 7)

struct MenuItem {
    std::string name;
    int priority;

    std::variant<Menu::Func, std::vector<MenuItem>> sub_items_function;
};

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
                    auto f = std::get<0>(i.sub_items_function);
                    if (f)f({});
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

//TODO: Replace addItem and addPlaceHolder with one helper function
void Menu::addItem(std::string path, Menu::Func f, int priority) {

    std::function<MenuItem &(std::vector<MenuItem> &, std::string)> s =
            [&](std::vector<MenuItem> &vector, std::string path) -> MenuItem & {
                auto last = path.rfind('/');
                auto name = GameApi::substr(path, '/');
                auto i = vector.begin();

                if (!path.empty() || last != std::string::npos) { //path exists
                    bool same_name = false;
                    for (auto it = vector.begin(); it != vector.end(); ++it) {
                        if (it->name == name) {
                            if (it->sub_items_function.index() == 1) { //this is vector
                                return s(std::get<1>(it->sub_items_function), path);
                            } else if (priority >= it->priority) {
                                i = it + 1;
                                same_name = true;
                            }
                        } else if (priority == it->priority && !same_name) {
                            i = it + 1;
                        } else if (priority > it->priority) {
                            i = it + 1;
                        }
                    }

                    return s(std::get<1>(
                            vector.emplace(i, MenuItem{name, priority, std::vector<MenuItem>()})->sub_items_function),
                             path);
                }

                for (auto it = vector.begin(); it != vector.end(); ++it) {
                    if (priority == it->priority && it->name == name
                        && it->sub_items_function.index() == 1) {
                        i = it;
                    } else if (priority >= it->priority) {
                        i = it + 1;
                    }
                }

                return *vector.emplace(i, MenuItem{name, priority, f});
            };

    s(global_item_vector(), std::move(path));
}

void Menu::addPlaceHolder(std::string path, int priority) {
    std::function<void(std::vector<MenuItem> &, std::string)> s =
            [&](std::vector<MenuItem> &vector, std::string path) {
                auto last = path.rfind('/');
                auto name = GameApi::substr(path, '/');
                auto i = vector.begin();

                if (!path.empty() || last != std::string::npos) { //path exists
                    bool same_name = false;
                    for (auto it = vector.begin(); it != vector.end(); ++it) {
                        if (it->name == name) {
                            if (it->sub_items_function.index() == 1) { //this is vector
                                return s(std::get<1>(it->sub_items_function), path);
                            } else if (priority >= it->priority) {
                                i = it + 1;
                                same_name = true;
                            }
                        } else if (priority == it->priority && !same_name) {
                            i = it + 1;
                        } else if (priority > it->priority) {
                            i = it + 1;
                        }
                    }

                    return s(std::get<1>(
                            vector.emplace(i, MenuItem{name, priority, std::vector<MenuItem>()})->sub_items_function),
                             path);
                }

                bool same_name = false;
                for (auto it = vector.begin(); it != vector.end(); ++it) {
                    if (it->name == name) {
                        if (it->sub_items_function.index() == 1) { //this is vector
                            return;
                        } else if (priority >= it->priority) {
                            i = it + 1;
                            same_name = true;
                        }
                    } else if (priority == it->priority && !same_name) {
                        i = it + 1;
                    } else if (priority > it->priority) {
                        i = it + 1;
                    }
                }

                vector.emplace(i, MenuItem{name, priority, std::vector<MenuItem>()});
            };

    s(global_item_vector(), std::move(path));
}

/** Debug display of items

void display(std::vector<MenuItem> items, std::string str = ""){
    for(auto& i : items){
        std::cout << str << i.name << ": ";
        if(i.sub_items_function.index() == 0){
            std::cout << "error\n";
        }else if(i.sub_items_function.index() == 1){
            void* p = (void*)std::get<1>(i.sub_items_function);
            std::cout << "func: " << p << "\n";
        }else if(i.sub_items_function.index() == 2){
            std::cout << "\n";
            display(std::get<2>(i.sub_items_function), str + "   ");
        }else{
            std::terminate();
        }
    }
}
 */
