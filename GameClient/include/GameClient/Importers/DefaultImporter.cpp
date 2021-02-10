//
// Created by Michal_Marszalek on 10.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Editor/SceneAsset.h>
#include <Editor/AssetDatabase.h>
#include <GameClient/GlobalStaticVariables.h>

class DefaultImporter : public AssetImporter {
    void OnImportAsset(AssetImportContext &ctx) override {
        ctx.AddObjectToAsset(0, std::make_shared<SceneAsset>());

        auto guid = AssetDatabase::AssetPathToGUID(ctx.assetPath);
        //Change guid when reloading asset when guid changed when scene was loaded, remove when dependency loading implemented
        //TODO: dependency loading implemented, remove
        for (auto &it : global.scene.data) {
            if (it.second.path == ctx.assetPath) {
                it.second.guid = guid;
            }
        }
    }
};

EXPORT_IMPORTER(DefaultImporter, (".unity", -200))
