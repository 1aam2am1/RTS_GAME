//
// Created by Michal_Marszalek on 07.12.2020.
//

#include "DragDropWindow.h"
#include <GameClient/Unity/Editor/DragAndDrop.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <GameClient/MainThread.h>
#include <GameClient/IconsFontAwesome5_c.h>

void DragDropWindow::Update() {
    TPtr<DragDropWindow> ob = shared_from_this();
    MainThread::Invoke([ob]() {
        if (ob) ob->m_visualMode = DragAndDrop::visualMode;

        DragAndDrop::visualMode = DragAndDrop::Rejected;
    });
}

void DragDropWindow::OnGUI() {

    switch (m_visualMode) {
        case DragAndDrop::None:
            break;
        case DragAndDrop::Copy:
            ImGui::Text(ICON_FA_SAVE);
            break;
        case DragAndDrop::Generic:
            ImGui::Text(ICON_FA_BORDER_STYLE);
            break;
        case DragAndDrop::Link:
            ImGui::Text(ICON_FA_EXTERNAL_LINK_ALT);
            break;
        case DragAndDrop::Move:
            ImGui::Text(ICON_FA_UPLOAD);
            break;
        case DragAndDrop::Rejected:
            ImGui::Text(ICON_FA_WINDOW_CLOSE);
    }
    ImGui::SameLine();
    ImGui::Text("%s", titleContent.data());
}

void DragDropWindow::drawGui() {
    if (work == NotShown) {
        Close();
        return;
    }

    OnStyleChange();
    if (ImGui::IsMouseDown(0) && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceExtern)) {
        this->OnGUI();

        ImGui::EndDragDropSource();
    } else {
        Close();
    }
    OnStylePop();
}
