//
// Created by Michal_Marszalek on 23.09.2020.
//

#include "EditorWindow.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <GameClient/GarbageCollector.h>

std::vector<std::shared_ptr<EditorWindow>> &EditorWindow::get_open_windows() {
    static std::vector<std::shared_ptr<EditorWindow>> list;
    return list;
}

EditorWindow::EditorWindow()
        : position([this](sf::FloatRect p) {
    if (ImGuiWindow *window = ImGui::FindWindowByName(imGuiName.c_str())) {
        ImGui::DockContextQueueUndockWindow(ImGui::GetCurrentContext(), window);
        ImGui::SetWindowPos(window, p.getPosition());
        ImGui::SetWindowSize(window, p.getSize());
    } else {
        auto s = ImGui::FindOrCreateWindowSettings(imGuiName.c_str());
        s->Pos.x = p.getPosition().x;
        s->Pos.y = p.getPosition().y;
        s->Size.x = p.getSize().x;
        s->Size.y = p.getSize().y;
    }
}, [this]() -> auto {
    ImVec2 p{}, s{};
    if (ImGuiWindow *window = ImGui::FindWindowByName(imGuiName.c_str())) {
        s = window->Size;
        p = window->Pos;
    } else {
        auto set = ImGui::FindOrCreateWindowSettings(imGuiName.c_str());
        p.x = set->Pos.x;
        p.y = set->Pos.y;
        s.x = set->Size.x;
        s.y = set->Size.y;
    }
    return sf::FloatRect{p, s};
}), imGuiName{"###" + GameApi::to_string(this)} {

}

std::shared_ptr<EditorWindow> EditorWindow::focusedWindow() {
    auto w = ImGui::GetCurrentContext()->NavWindow;
    if (w) {
        std::string s(w->Name);
        auto p = GameApi::to_pointer(GameApi::rsubstr(s, '#'));
        auto &v = get_open_windows();
        auto f = std::find_if(v.begin(), v.end(), [p](const auto &it) { return p == it.get(); });
        if (f != v.end()) { return *f; }
    }

    return {};
}

std::shared_ptr<EditorWindow> EditorWindow::mouseOverWindow() {
    auto w = ImGui::GetCurrentContext()->HoveredWindow;
    if (w) {
        std::string s(w->Name);
        auto p = GameApi::to_pointer(GameApi::rsubstr(s, '#'));
        auto &v = get_open_windows();
        auto f = std::find_if(v.begin(), v.end(), [p](const auto &it) { return p == it.get(); });
        if (f != v.end()) { return *f; }
    }

    return {};
}

void EditorWindow::Close() {
    work = NotShown;
    ///? Destroy ?
    auto s = shared_from_this();
    GarbageCollector::add(s, [](auto s) {
        auto &vec = get_open_windows();
        auto it = std::find_if(vec.begin(), vec.end(), [&](auto &e) { return e == s; });
        if (it != vec.end()) { vec.erase(it); }
    });

}

void EditorWindow::Focus() {
    switch (work) {
        case Normal:
        case Utility:
            ImGui::SetWindowFocus(imGuiName.c_str());
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
            OnStyleChange();
            if (ImGui::Begin((titleContent + imGuiName).c_str(), &b, flags)) {
                this->OnGUI();
            }

            ImGui::End();
            OnStylePop();

            if (!b) { Close(); }
        }
            break;
        case Utility: {
            bool b = true;

            OnStyleChange();
            if (ImGui::Begin((titleContent + imGuiName).c_str(),
                             &b, flags | ImGuiWindowFlags_NoDocking)) {
                this->OnGUI();
            }

            ImGui::End();
            OnStylePop();

            if (!b) { Close(); }
        }
            break;
        case DropDown: {
            ///ImGui::OpenPopup

            OnStyleChange();
            if (ImGui::BeginPopup((titleContent + imGuiName).c_str(), flags)) {
                this->OnGUI();

                ImGui::EndPopup();
            } else {
                Close();
            }
            OnStylePop();
        }
            break;
        case Popup: {
            ///ImGui::OpenPopup

            OnStyleChange();
            if (ImGui::BeginPopupModal((titleContent + imGuiName).c_str(), nullptr,
                                       flags | ImGuiWindowFlags_AlwaysAutoResize)) {
                this->OnGUI();

                ImGui::EndPopup();
            } else {
                Close();
            }
            OnStylePop();
        }
            break;
        case NotShown: {

        }
            break;
    }
}
