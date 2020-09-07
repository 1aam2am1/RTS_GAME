//
// Created by Michal_Marszalek on 11.08.2020.
//

#include <atomic>
#include "ThreadId.h"

namespace GameApi {

    static std::atomic<int> count;

    int getCurrentThreadId() {
        thread_local int id = count++;

        return id;
    }

}
