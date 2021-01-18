//
// Created by Michal_Marszalek on 01.11.2020.
//

#include "AssetImporter.h"
#include "ImportAssetOptions.h"
#include "AssetDatabase.h"
#include "EditorUtility.h"
#include <GameClient/Unity/Macro.h>
#include <GameClient/MetaData.h>
#include <filesystem>
#include <GameClient/Unity/Editor/OneGuidFile.h>
#include <GameClient/GlobalStaticVariables.h>

namespace fs = std::filesystem;

EXPORT_CLASS(AssetImporter, ("m_userData", userData))

extern void SaveAsset(Unity::GUID g, OneGUIDFile *o);

extern std::pair<Unity::GUID, OneGUIDFile> ImportAssetGlobal(std::string assetPath, ImportAssetOptions options);

void AssetImporter::SaveAndReimport() {
    if (!EditorUtility::IsDirty(shared_from_this())) { return; }

    auto it = std::find_if(global.assets.objects.begin(), global.assets.objects.end(),
                           [&](auto &p) { return p.second.importer.get() == this; });
    if (it != global.assets.objects.end()) {
        SaveAsset(it->first, &it->second);
        ImportAssetGlobal(it->second.path, ImportAssetOptions::ForceUpdate);
    }
}

TPtr<AssetImporter> AssetImporter::GetAtPath(std::string path) {
    auto guid = AssetDatabase::AssetPathToGUID(path);
    if (guid.empty()) {
        return {};
    }

    auto it = global.assets.objects.find(guid);
    if (it != global.assets.objects.end()) { return it->second.importer; }

    return {};
}

void AssetImporter::OnExportAsset(AssetImportContext &) {}
