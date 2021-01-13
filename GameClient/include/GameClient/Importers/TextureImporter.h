//
// Created by Michal_Marszalek on 13.01.2021.
//

#ifndef RTS_GAME_TEXTUREIMPORTER_H
#define RTS_GAME_TEXTUREIMPORTER_H

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/Texture2D.h>
#include <GameClient/Unity/Core/Sprite.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>


class TextureImporter : public AssetImporter {
public:
    std::vector<nlohmann::json> sprites{};
    float pixelsPerUnit = 32.f;
    bool force_sprites = false;

    void OnImportAsset(AssetImportContext &ctx) override;

    void OnExportAsset(AssetImportContext &ctx) override;

};

#endif //RTS_GAME_TEXTUREIMPORTER_H
