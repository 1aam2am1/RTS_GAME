//
// Created by Michal_Marszalek on 25.11.2020.
//

#ifndef RTS_GAME_MENUITEM_H
#define RTS_GAME_MENUITEM_H

#include <string>
#include <functional>
#include <GameClient/Unity/Editor/MenuCommand.h>

struct MenuItem {
    std::string name{};
    int priority{};

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

        std::vector<std::function<bool(MenuCommand)>> validation{};
        std::vector<std::function<bool(MenuCommand)>> execution{};
    };

    std::variant<Func, std::vector<MenuItem>> sub_items_function;
};

inline MenuItem &get_node(std::vector<MenuItem> &vector, std::string path, const int priority, const bool function) {
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


#endif //RTS_GAME_MENUITEM_H
