//
// Created by Michal_Marszalek on 26.10.2020.
//

#include "EditorUtility.h"

void EditorUtility::SetDirty(TPtr<Object> target) {

}

void EditorUtility::ClearDirty(TPtr<Object> target) {

}

bool EditorUtility::IsDirty(TPtr<Object> target) {
    //TODO: Make that so sub assets make main asset dirty and prefab changed, game object, component make scene dirty
    return false;
}
