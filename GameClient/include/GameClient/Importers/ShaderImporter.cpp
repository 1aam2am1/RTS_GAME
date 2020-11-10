//
// Created by Michal_Marszalek on 10.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>

class ShaderImporter : public AssetImporter {
    void OnImportAsset(AssetImportContext &ctx) override {
        //TODO: Import Shader based on type and with default textures
    }
};

EXPORT_IMPORTER(ShaderImporter, (".shader", ".frag", ".geom", ".vert", -200))
