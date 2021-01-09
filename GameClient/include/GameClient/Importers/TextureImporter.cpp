//
// Created by Michal_Marszalek on 02.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Texture2D.h>
#include <GameClient/Unity/Core/Sprite.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>

class TextureImporter : public AssetImporter {
public:
    std::vector<nlohmann::json> sprites{};

    void OnImportAsset(AssetImportContext &ctx) override {
        TPtr<Texture2D> texture = std::make_shared<Texture2D>();
        texture->t0.loadFromFile(ctx.assetPath);
        ctx.AddObjectToAsset(0, texture, texture);
        ctx.SetMainObject(texture);

        if (sprites.empty()) {
            ctx.AddObjectToAsset(1, Sprite::Create(texture, {0, 0, static_cast<float>(texture->t0.getSize().x),
                                                             static_cast<float>(texture->t0.getSize().y)}));
        }

        for (auto s: sprites) {
            auto p = s["pivot"];
            sf::Vector2f pivot;
            p["x"].get_to(pivot.x);
            p["y"].get_to(pivot.y);

            auto r = s["rect"];
            sf::FloatRect rect;
            r["left"].get_to(rect.left);
            r["top"].get_to(rect.top);
            r["width"].get_to(rect.width);
            r["height"].get_to(rect.height);

            auto id = s["id"].get<Unity::fileID>();
            auto pixelsPerUnit = s["pixelsPerUnit"].get<float>();

            ctx.AddObjectToAsset(id, Sprite::Create(texture, rect, pivot, pixelsPerUnit));
        }

    }

    void OnExportAsset(AssetImportContext &ctx) override {
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

            auto &rect = j["rect"];
            rect["left"] = sprite->s0.getTextureRect().left;
            rect["top"] = sprite->s0.getTextureRect().top;
            rect["width"] = sprite->s0.getTextureRect().width;
            rect["height"] = sprite->s0.getTextureRect().height;

            {
                Unity::GUID g;
                Unity::fileID id;
                if (!AssetDatabase::TryGetGUIDAndLocalFileIdentifier(sprite, g, id)) {
                    id = Unity::GUID::NewGuid().second;
                }

                j["id"] = id;
            }

            j["pixelsPerUnit"] = sprite->pixelsPerUnit;

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
};

EXPORT_IMPORTER(TextureImporter, (".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic", -100),
                ("spriteSheet", sprites))
