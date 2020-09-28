//
// Created by Michal_Marszalek on 23.09.2020.
//

#include "EditorWindow.h"
#include <imgui.h>

std::vector<std::shared_ptr<EditorWindow>> &EditorWindow::get_open_windows() {
    static std::vector<std::shared_ptr<EditorWindow>> list;
    return list;
}

EditorWindow::EditorWindow() : position([this](auto) {}, [this]() -> auto { return sf::FloatRect(); }) {

}

void EditorWindow::Close() {
    //TODO: Remove from shared_windows and delete outside gui loop
}

void EditorWindow::Focus() {
    switch (work) {
        case Normal:
        case Utility:
            ImGui::SetWindowFocus(("###" + GameApi::to_string(this)).c_str());
            break;
        case NotShown:
        case DropDown:
        case Popup:
            break;
    }
}

void EditorWindow::Show() {
    if (work == NotShown) { work = Normal; }
}

void EditorWindow::ShowUtility() {
    if (work == NotShown) { work = Utility; }
}

void EditorWindow::drawGui() {
    if (work != NotShown) {
        ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
    }

    switch (work) {
        case Normal: {
            bool b = true;
            if (ImGui::Begin((titleContent + "###" + GameApi::to_string(this)).c_str(), &b)) {
                this->OnGUI();
            }

            ImGui::End();

            if (!b) { Close(); }
        }
            break;
        case Utility: {
            bool b = true;
            if (ImGui::Begin((titleContent + "###" + GameApi::to_string(this)).c_str(),
                             &b, ImGuiWindowFlags_NoDocking)) {
                this->OnGUI();
            }

            ImGui::End();

            if (!b) { Close(); }
        }
            break;
        case DropDown: {
            ///ImGui::OpenPopup
            if (ImGui::BeginPopup((titleContent + "###" + GameApi::to_string(this)).c_str())) {
                this->OnGUI();

                ImGui::EndPopup();
            } else {
                Close();
            }
        }
            break;
        case Popup: {
            ///ImGui::OpenPopup
            if (ImGui::BeginPopupModal((titleContent + "###" + GameApi::to_string(this)).c_str(), nullptr,
                                       ImGuiWindowFlags_AlwaysAutoResize)) {
                this->OnGUI();

                ImGui::EndPopup();
            } else {
                Close();
            }
        }
            break;
        case NotShown: {

        }
            break;
    }
}
