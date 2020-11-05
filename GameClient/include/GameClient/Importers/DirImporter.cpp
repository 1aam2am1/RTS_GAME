//
// Created by Michal_Marszalek on 01.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>

class DirImporter : public AssetImporter {
public:
    void OnImportAsset(TPtr<AssetImportContext> ctx) override {
        UNUSED(ctx);
    }

    void OnExportAsset(TPtr<AssetImportContext> ctx) override {
        UNUSED(ctx);
    }
};

EXPORT_IMPORTER(DirImporter, ("_UNITY/D", -200))
