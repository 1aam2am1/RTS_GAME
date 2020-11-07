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
    nlohmann::json sprites;

    void OnImportAsset(AssetImportContext &ctx) override {
        TPtr<Texture2D> texture{nullptr, std::make_shared<Texture2D>()};
        texture->t0.loadFromFile(ctx.assetPath);
        ctx.AddObjectToAsset(0, texture, texture);
        ctx.SetMainObject(texture);

        std::list<TPtr<Sprite>> no_ids;
        std::vector<Unity::fileID> ids;
        ids.push_back(0);

        for (auto &it: sprites) {
            sf::FloatRect rect = {0, 0, static_cast<float>(texture->t0.getSize().x),
                                  static_cast<float>(texture->t0.getSize().y)};
            auto rect_j = it.find("rect");
            if (rect_j != it.end()) {
                (*rect_j)["left"].get_to(rect.left);
                (*rect_j)["top"].get_to(rect.top);
                (*rect_j)["width"].get_to(rect.width);
                (*rect_j)["height"].get_to(rect.height);
            }

            auto s = Sprite::Create(texture, rect);

            auto id_j = it.find("spriteID");
            Unity::fileID id;
            if (id_j != it.end()) {
                id_j->get_to(id);

                ids.push_back(id);
                ctx.AddObjectToAsset(id, s);
            } else {
                no_ids.push_back(s);
            }
        }

        Unity::fileID id = Unity::GUID::NewGuid().second;
        for (auto &it: no_ids) {
            while (std::find_if(ids.begin(), ids.end(), [id](auto &&i) { return i == id; }) != ids.end()) {
                ++id;
            }
            ctx.AddObjectToAsset(id, it);
            ids.push_back(id);
        }
    }

    void OnExportAsset(AssetImportContext &ctx) override {
        std::vector<TPtr<Object>> objects;
        ctx.GetObjects(objects);

        auto main_texture = dynamic_pointer_cast<Texture2D>(ctx.mainObject);

        std::list<TPtr<Sprite>> no_ids;
        std::vector<Unity::fileID> ids;
        ids.push_back(0);

        sprites.clear();
        for (auto &it: objects) {
            auto texture = dynamic_pointer_cast<Texture2D>(it);
            auto sprite = dynamic_pointer_cast<Sprite>(it);

            if (texture) {
                if (!main_texture) {
                    main_texture = texture;
                }
            } else if (sprite) {
                nlohmann::json j;

                Unity::GUID g;
                Unity::fileID id;
                if (!AssetDatabase::TryGetGUIDAndLocalFileIdentifier(sprite, g, id)) {
                    no_ids.push_back(sprite);
                    break;
                }

                ids.push_back(id);

                j["spriteID"] = id;
                j["rect"]["left"] = sprite->s0.getTextureRect().left;
                j["rect"]["top"] = sprite->s0.getTextureRect().top;
                j["rect"]["width"] = sprite->s0.getTextureRect().width;
                j["rect"]["height"] = sprite->s0.getTextureRect().height;

                sprites.push_back(j);
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

        Unity::fileID id = Unity::GUID::NewGuid().second;
        for (auto &it: no_ids) {
            while (std::find_if(ids.begin(), ids.end(), [id](auto &&i) { return i == id; }) != ids.end()) {
                ++id;
            }
            ids.push_back(id);

            nlohmann::json j;

            j["spriteID"] = id;
            j["rect"]["left"] = it->s0.getTextureRect().left;
            j["rect"]["top"] = it->s0.getTextureRect().top;
            j["rect"]["width"] = it->s0.getTextureRect().width;
            j["rect"]["height"] = it->s0.getTextureRect().height;

            sprites.push_back(j);
        }

    }
};

EXPORT_IMPORTER(TextureImporter, (".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic", -100),
                ("spriteSheet", sprites))
