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
        auto type = MetaData::get_importer(extension);

        return dynamic_pointer_cast<AssetImporter>(MetaData::get_name_constructor(type.first).second->create());
    } EXCEPTION_PRINT

    return TPtr<AssetImporter>{nullptr};
}
