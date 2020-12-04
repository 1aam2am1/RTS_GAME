//
// Created by Michal_Marszalek on 10.11.2020.
//

#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Core/TextAsset.h>

class TextScriptImporter : public AssetImporter {
    void OnImportAsset(AssetImportContext &ctx) override {
        auto txt = GameApi::readFullFile(ctx.assetPath);

        TPtr<Object> main = std::make_shared<TextAsset>(txt);

        ctx.AddObjectToAsset(0, main);
    }

    void OnExportAsset(AssetImportContext &ctx) override {
        auto ptr = dynamic_pointer_cast<TextAsset>(ctx.mainObject);
        std::string txt;
        if (ptr) { txt = ptr->text; }
        GameApi::saveFullFile(ctx.assetPath, txt);
    }
};

EXPORT_IMPORTER(TextScriptImporter,
                (".txt", ".html", ".htm", ".xml", ".json", ".csv", ".yaml",
                        ".bytes", ".fnt", ".manifest", ".md", ".js", ".boo", ".rsp", -300))
