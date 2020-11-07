//
// Created by Michal_Marszalek on 30.10.2020.
//

#include "Initializer.h"
#include <list>

static auto &get_data() {
    static std::pair<bool, std::list<std::function<void()>>> result{false, {}};
    return result;
}

static auto &get_uint__data() {
    static std::pair<bool, std::list<std::function<void()>>> result{false, {}};
    return result;
}

int Initializer::add(std::function<void()> f) {
    auto &r = get_data();
    if (r.first) {
        f();
        return 1;
    } else {
        r.second.emplace_back(f);
        return 0;
    }
}

void Initializer::initialize() {
    auto &r = get_data();
    r.first = true;
    for (auto &it: r.second) {
        if (it)it();
    }
    r.second.clear();
}

int Initializer::d_add(std::function<void()> f) {
    auto &r = get_uint__data();
    if (r.first) {
        f();
        return 1;
    } else {
        r.second.emplace_back(f);
        return 0;
    }
}

void Initializer::uninitialize() {
    auto &r = get_uint__data();
    r.first = true;
    for (auto &it: r.second) {
        if (it)it();
    }
    r.second.clear();
}
