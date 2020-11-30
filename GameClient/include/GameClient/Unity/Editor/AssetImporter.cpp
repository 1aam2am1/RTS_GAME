//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "AssetImporter.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/MetaData.h>
#include <filesystem>

namespace fs = std::filesystem;

EXPORT_CLASS(AssetImporter, ("m_userData", userData))

void AssetImporter::SaveAndReimport() {

}

TPtr<AssetImporter> AssetImporter::GetAtPath(std::string path) {
    fs::path p(path);

    auto extension = p.extension().string();
    try {
        auto type = Importers::get_importer(extension);

        return dynamic_pointer_cast<AssetImporter>(MetaData::getReflection(type.first).CreateInstance());
    } EXCEPTION_PRINT

    return TPtr<AssetImporter>{nullptr};
}

void AssetImporter::OnExportAsset(AssetImportContext &ctx) {}
