//
// Created by Michal_Marszalek on 28.09.2020.
//

#ifndef RTS_GAME_ASSETDATABASE_H
#define RTS_GAME_ASSETDATABASE_H

#include <vector>
#include <string_view>
#include <string>
#include <GameClient/Unity/Core/Texture2D.h>
#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Guid.h>
#include "ImportAssetOptions.h"

class AssetDatabase {
public:

    /// Given a path to a directory in the Assets folder, relative to the project folder,
    /// this method will return an array of all its subdirectories.
    /// \details Assets/.
    static std::vector<std::string> GetSubFolders(std::string path);

    /// Returns an array of all the assets that are dependencies of the asset at the specified pathName.
    /// \param pathName The path to the asset for which dependencies are required.
    /// \param recursive Controls whether this method recursively checks and returns all dependencies including indirect dependencies
    /// (when set to true), or whether it only returns direct dependencies (when set to false).
    /// \return The paths of all assets that the input depends on.
    static std::vector<std::string> GetDependencies(std::string pathName, bool recursive);

    /// Retrieves an icon for the asset at the given asset path.
    static TPtr<Texture2D> GetCachedIcon(std::string path);

    /// Returns an array of all Assets at assetPath.
    /// Some Asset files may contain multiple sub Assets
    /// \param assetPath Filesystem path to the asset.
    static std::vector<TPtr<Object>> LoadAllAssetsAtPath(std::string assetPath);

    /// Returns all sub Assets at assetPath.
    /// \param assetPath Filesystem path to the asset.
    static std::vector<TPtr<Object>> LoadAllAssetRepresentationsAtPath(std::string assetPath);

    /// Does the asset form part of another asset?
    /// \param obj The asset Object to query.
    static bool IsSubAsset(Object *obj);

    /// Is asset a main asset in the project window?
    static bool IsMainAsset(Object *obj);

    /// Is object an asset?
    static bool Contains(Object *obj);

    /// Returns true if the main asset object at assetPath is loaded in memory.
    static bool IsMainAssetAtPathLoaded(std::string assetPath);

    /// Given a path to a folder, returns true if it exists, false otherwise.
    static bool IsValidFolder(std::string path);

    /// Creates a new unique path for an asset.
    static std::string GenerateUniqueAssetPath(std::string path);

    /// Returns the path name relative to the project folder where the asset is stored.
    static std::string GetAssetOrScenePath(Object *assetObject);

    /// Returns the path name relative to the project folder where the asset is stored.
    /// \return The asset path name, or an empty string if the asset does not exist.
    static std::string GetAssetPath(Object *assetObject);

    /// Gets the corresponding asset path for the supplied guid, or an empty string if the GUID can't be found.
    static std::string GUIDToAssetPath(Unity::GUID guid);

    /// Get the GUID for the asset at path.
    static Unity::GUID AssetPathToGUID(std::string path);

    /// Import asset at path.
    static void ImportAsset(std::string path, ImportAssetOptions options = ImportAssetOptions::Default);

    /// Returns the first asset object of type type at given path assetPath.
    static TPtr<Object> LoadAssetAtPath(std::string assetPath, std::type_info type);

    /// Returns the main asset object at assetPath.
    static TPtr<Object> LoadMainAssetAtPath(std::string assetPath);

    /// Move an asset file (or folder) from one folder to another.
    /// \return An empty string if the asset has been successfully moved, otherwise an error message.
    static std::string MoveAsset(std::string oldPath, std::string newPath);

    /// Duplicates the asset at path and stores it at newPath.
    static bool CopyAsset(std::string path, std::string newPath);

    /// Moves the asset at path to the trash.
    /// \return true if the asset has been successfully removed,
    /// false if it doesn't exist or couldn't be moved to the trash.
    static bool MoveAssetToTrash(std::string path);

    /// Rename an asset file.
    /// \return An empty string, if the asset has been successfully renamed, otherwise an error message.
    static std::string RenameAsset(std::string pathName, std::string newName);

    /// Writes all unsaved asset changes to disk.
    static void SaveAssets();

    /// Opens the asset with associated application.
    static bool OpenAsset(Object *target);

    /// Import any changed assets.
    /// This will import any assets that have changed their content modification data or have been added-removed to the project folder.
    /// This method implicitly triggers an asset garbage collection
    static void Refresh(ImportAssetOptions options = ImportAssetOptions::Default);

    /// Creates a new asset at path.
    /// You must ensure that the path uses a supported extension.
    /// \details If an asset already exists at path it will be deleted prior to creating a new asset.
    static void CreateAsset(Object *asset, std::string path);

    /// Deletes the asset file at path.
    /// \return true if the asset has been successfully deleted, false if it doesn't exist or couldn't be removed.
    static bool DeleteAsset(std::string path);

    /// Create a new folder.
    static Unity::GUID CreateFolder(std::string parentFolder, std::string newFolderName);

    /// Increments an internal counter which Unity uses to determine whether to allow automatic AssetDatabase refreshing behavior.
    ///static void DisallowAutoRefresh();

    /// ecrements an internal counter which Unity uses to determine whether to allow automatic AssetDatabase refreshing behavior.
    ///static void AllowAutoRefresh();

    /// Callback raised whenever a package import starts.
    sigslot::signal<> importPackageStarted;

    /// Callback raised whenever a package import successfully completes.
    sigslot::signal<> importPackageCompleted;
};


#endif //RTS_GAME_ASSETDATABASE_H
