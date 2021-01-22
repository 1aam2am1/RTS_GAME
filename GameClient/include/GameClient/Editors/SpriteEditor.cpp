#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Core/Sprite.h>
#include <GameClient/Unity/Macro.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui-SFML.h>
#include <Editor/AssetImporter.h>

class SpriteEditor : public Editor {
public:
    TPtr<Sprite> d;
    TPtr<AssetImporter> importer;

    void Awake() override {
        d = dynamic_pointer_cast<Sprite>(target);
    }

    void OnInspectorGUI() override {
        if (!d) { return; }

        float width = ImGui::GetContentRegionAvail().x;
        if (!ImGui::GetCurrentWindow()->ScrollbarY) {
            width = width - ImGui::GetStyle().ScrollbarSize;
        }

        sf::Vector2f size{d->s0.getTextureRect().getSize()};

        if (size.x != 0) {
            size.y = (width / size.x) * size.y;
            size.x = width;
        }

        ImGui::Image(d->s0, size);

        ImGui::Text("pixelsPerUnit: %u", d->pixelsPerUnit);

        ImGui::TextWrapped("X: %d, Y: %d\nW: %d, H: %d", d->s0.getTextureRect().left,
                           d->s0.getTextureRect().top,
                           d->s0.getTextureRect().width,
                           d->s0.getTextureRect().height);
    }
};

CUSTOM_EDITOR_FALLBACK(SpriteEditor, Sprite);
