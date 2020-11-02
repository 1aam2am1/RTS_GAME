//
// Created by Michal_Marszalek on 01.11.2020.
//

#ifndef RTS_GAME_ASSETIMPORTER_H
#define RTS_GAME_ASSETIMPORTER_H

#include <GameClient/Unity/Core/Object.h>
#include <GameClient/TPtr.h>
#include <GameClient/Unity/Editor/AssetImportContext.h>

class AssetDatabase;

class AssetImporter : public Object {
public:

    /// The path name of the asset for this importer
    const std::string &assetPath = m_assetPath;

    /// The value is true when no meta file is provided with the imported asset.
    bool importSettingsMissing;

    /// Get or set any user data.
    std::string userData;

    /// Save asset importer settings if asset importer is dirty.
    virtual void SaveAndReimport();

    /// Retrieves the asset importer for the asset at path.
    static TPtr<AssetImporter> GetAtPath(std::string path);

    /// This method must by overriden by the derived class and is called by the Asset pipeline to import files.
    void OnImportAsset(TPtr<AssetImportContext> ctx);

private:
    friend class AssetDatabase;

    std::string m_assetPath;
};


#endif //RTS_GAME_ASSETIMPORTER_H
