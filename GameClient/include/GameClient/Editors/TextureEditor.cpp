//
// Created by Michal_Marszalek on 19.11.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/Texture2D.h>
#include <GameClient/Unity/Macro.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>

class TextureEditor : public Editor {
public:
    void OnInspectorGUI() override {
        auto d = dynamic_pointer_cast<Texture2D>(target);
        if (!d) { return; }

        float width = ImGui::GetContentRegionAvail().x;
        if (!ImGui::GetCurrentWindow()->ScrollbarY) {
            width = width - ImGui::GetStyle().ScrollbarSize;
        }

        sf::Vector2f size{d->t0.getSize()};

        if (d->t0.getSize().x != 0) {
            size.y = (width / size.x) * size.y;
            size.x = width;
        }

        ImGui::Image(d->t0, size);

        DrawDefaultInspector();
    }
};

CUSTOM_EDITOR_FALLBACK(TextureEditor, Texture2D);
