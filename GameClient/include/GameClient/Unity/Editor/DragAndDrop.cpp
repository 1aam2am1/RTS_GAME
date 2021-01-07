//
// Created by Michal_Marszalek on 07.12.2020.
//

#include "DragAndDrop.h"
#include <unordered_map>
#include <GameClient/Windows/DragDropWindow.h>

decltype(DragAndDrop::paths) DragAndDrop::paths;
decltype(DragAndDrop::objectReferences) DragAndDrop::objectReferences;
decltype(DragAndDrop::visualMode) DragAndDrop::visualMode;

std::unordered_map<std::string, TPtr<>> generic;

void DragAndDrop::PrepareStartDrag() {
    if (EditorWindow::HasOpenInstances(typeid(DragDropWindow))) {
        EditorWindow::GetWindow<DragDropWindow>()->Close();
    }

    paths.clear();
    objectReferences.clear();
    generic.clear();

    visualMode = DragAndDrop::Rejected;
}

void DragAndDrop::SetGenericData(std::string_view type, TPtr<> data) {
    generic[std::string{type}] = data;
    objectReferences.emplace_back(data);
}

TPtr<> DragAndDrop::GetGenericData(std::string_view type) {
    auto it = generic.find(std::string{type});
    if (it != generic.end()) {
        return it->second;
    }
    return TPtr<>();
}

static TPtr<EditorWindow> window;

bool DragAndDrop::IsDragging() {
    return !window.expired();
}

void DragAndDrop::AcceptDrag() {
    if (window) {
        window->Close();
    }
}

void DragAndDrop::StartDrag(std::string_view title) {
    window = DragDropWindow::Init(title);
}


