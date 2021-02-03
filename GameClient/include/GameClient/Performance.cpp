//
// Created by Michal_Marszalek on 2021-02-03.
//

#include "Performance.h"

Performance performance{};


std::chrono::microseconds Performance::GetData(Performance::Break b) {
    return tab[(int) b];
}

void Performance::RefreshData(Performance::Break b) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now - last;
    last = now;

    tab2[(int) b] += std::chrono::duration_cast<std::chrono::milliseconds>(duration);
}

void Performance::reset() {
    for (size_t i = 0; i < tab_size; ++i) {
        tab[i] = tab2[i];
        tab2[i] = {};
    }
}
