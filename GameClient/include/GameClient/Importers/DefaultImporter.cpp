//
// Created by Michal_Marszalek on 10.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/SceneAsset.h>

class DefaultImporter : public AssetImporter {
    void OnImportAsset(AssetImportContext &ctx) override {
        ctx.AddObjectToAsset(0, TPtr<Object>{nullptr, std::make_shared<SceneAsset>()});
    }
};

EXPORT_IMPORTER(DefaultImporter, (".unity", -200))
