//
// Created by Michal_Marszalek on 02.11.2020.
//


#include <GameClient/Unity/Serialization/to_json.h>
#include "TextureImporter.h"
#include <filesystem>

namespace fs = std::filesystem;

void TextureImporter::OnImportAsset(AssetImportContext &ctx) {
    TPtr<Texture2D> texture = std::make_shared<Texture2D>();
    texture->t0.loadFromFile(ctx.assetPath);
    texture->name = ctx.name;
    ctx.AddObjectToAsset(0, texture, texture);
    ctx.SetMainObject(texture);

    if (sprites.empty()) {
        ctx.AddObjectToAsset(1, Sprite::Create(texture, {0, 0, static_cast<float>(texture->t0.getSize().x),
                                                         static_cast<float>(texture->t0.getSize().y)}));
    }

    int i = 1;
    for (auto s : sprites) {
        auto p = s["pivot"];
        sf::Vector2f pivot;
        p["x"].get_to(pivot.x);
        p["y"].get_to(pivot.y);

        sf::FloatRect rect = s["rect"].get<sf::FloatRect>();

        auto id = s["id"].get<Unity::fileID>();
        auto ppu = s["pixelsPerUnit"].get<float>();

        auto name = s.contains("name") ? s["name"].get<std::string>() :
                    (fs::path(assetPath).filename().generic_string() + "_" + GameApi::to_string(i));

        if (force_sprites) { ppu = pixelsPerUnit; }

        auto sprite = Sprite::Create(texture, rect, pivot, ppu);
        sprite->name = name;
        ctx.AddObjectToAsset(id, sprite);

        ++i;
    }

}

void TextureImporter::OnExportAsset(AssetImportContext &ctx) {
    std::vector<TPtr<Object>> objects;
    ctx.GetObjects(objects);

    auto main_texture = dynamic_pointer_cast<Texture2D>(ctx.mainObject);

    std::list<TPtr<Sprite>> no_ids;
    std::vector<Unity::fileID> ids;
    sprites.clear();

    auto to_json = [](TPtr<Sprite> sprite) {
        nlohmann::json j;

        auto &pivot = j["pivot"];
        pivot["x"] = sprite->s0.getOrigin().x / sprite->s0.getLocalBounds().getSize().x;
        pivot["y"] = sprite->s0.getOrigin().y / sprite->s0.getLocalBounds().getSize().y;

        j["rect"] = sprite->s0.getTextureRect();

        {
            Unity::GUID g;
            Unity::fileID id;
            if (!AssetDatabase::TryGetGUIDAndLocalFileIdentifier(sprite, g, id)) {
                id = Unity::GUID::NewGuid().second;
            }

            j["id"] = id;
        }

        j["pixelsPerUnit"] = sprite->pixelsPerUnit;
        j["name"] = sprite->name;

        return j;
    };

    for (auto &it: objects) {
        auto texture = dynamic_pointer_cast<Texture2D>(it);
        auto sprite = dynamic_pointer_cast<Sprite>(it);

        if (texture) {
            if (!main_texture) {
                main_texture = texture;
            }
        } else if (sprite) {
            sprites.emplace_back(to_json(sprite));
        } else {
            GameApi::log(ERR.fmt("Used TextureImporter with wrong type of object: %s",
                                 GameApi::demangle(typeid(*it.get()).name()).data()));
        }
    }

    if (!main_texture) {
        throw std::runtime_error("No texture when saving in TextureImporter");
    }
    if (importSettingsMissing) {
        main_texture->t0.copyToImage().saveToFile(ctx.assetPath);
    }
}


EXPORT_IMPORTER(TextureImporter, (".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic", -100),
                ("spriteSheet", sprites), pixelsPerUnit, force_sprites)
