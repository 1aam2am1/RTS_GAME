//
// Created by Michal_Marszalek on 14.09.2020.
//

#include "AsyncOperation.h"

YieldReturn AsyncOperation::operator()() {
    if (!isDone) {
        return nullptr;
    }
    return {};
}
