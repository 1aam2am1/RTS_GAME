//
// Created by Michal_Marszalek on 02.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>

class TrueTypeFontImporter : public AssetImporter {
    void OnImportAsset(AssetImportContext &ctx) override {

    }

    void OnExportAsset(AssetImportContext &ctx) override {

    }
};

EXPORT_IMPORTER(TrueTypeFontImporter, (".ttf", -100))
