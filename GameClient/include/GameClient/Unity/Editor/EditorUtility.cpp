//
// Created by Michal_Marszalek on 26.10.2020.
//

#include "EditorUtility.h"
#include <set>
#include <shared_mutex>

static std::set<TPtr<>> dirty{};
static std::shared_mutex m;

void EditorUtility::SetDirty(TPtr<Object> target) {
    std::unique_lock<std::shared_mutex> lock(m);
    dirty.emplace(target);
}

void EditorUtility::ClearDirty(TPtr<Object> target) {
    std::unique_lock<std::shared_mutex> lock(m);
    dirty.erase(TPtr<>{});
    dirty.erase(target);
}

bool EditorUtility::IsDirty(TPtr<Object> target) {
    //TODO: Make that so sub assets make main asset dirty and prefab changed, game object, component make scene dirty
    std::shared_lock<std::shared_mutex> lock(m);
    return target && dirty.contains(target);
}
