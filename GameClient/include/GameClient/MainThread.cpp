//
// Created by Michal_Marszalek on 01.10.2020.
//

#include "MainThread.h"

static std::vector<std::function<void()>> list[2];
static int i = 0;
static std::mutex m;

auto &getInvokedFunctions(bool change = false) {
    if (change) {
        int j = i;

        i = (i + 1) % 2;

        return list[j];
    } else {
        return list[i];
    }
}

void MainThread::Invoke(std::function<void()> f) {
    std::lock_guard<std::mutex> lock(m);
    getInvokedFunctions().emplace_back(f);
}

void MainThread::run() {
    std::vector<std::function<void()>> *copy = nullptr;
    {
        std::lock_guard<std::mutex> lock(m);

        copy = &getInvokedFunctions(true);
    }

    for (auto &object : *copy) {
        if (object)object();
    }

    {
        std::lock_guard<std::mutex> lock(m);
        copy->clear();
    }
}
