//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "AssetDatabase.h"
#include <map>
#include <filesystem>
#include <chrono>
#include <GameApi/StringFormatter.h>
#include <GameApi/GlobalLogSource.h>

#if defined(_WIN32)

#include <windows.h>

#endif

namespace fs = std::filesystem;


std::vector<std::string> AssetDatabase::GetSubFolders(std::string path) {
    return std::vector<std::string>();
}

std::vector<std::string> AssetDatabase::GetDependencies(std::string pathName, bool recursive) {
    return std::vector<std::string>();
}

TPtr<Texture2D> AssetDatabase::GetCachedIcon(std::string path) {
    return TPtr<Texture2D>(nullptr);
}

std::vector<TPtr<Object>> AssetDatabase::LoadAllAssetsAtPath(std::string assetPath) {
    return std::vector<TPtr<Object>>();
}

std::vector<TPtr<Object>> AssetDatabase::LoadAllAssetRepresentationsAtPath(std::string assetPath) {
    return std::vector<TPtr<Object>>();
}

bool AssetDatabase::IsSubAsset(Object *obj) {
    return false;
}

bool AssetDatabase::IsMainAsset(Object *obj) {
    return false;
}

bool AssetDatabase::Contains(Object *obj) {
    return false;
}

bool AssetDatabase::IsMainAssetAtPathLoaded(std::string assetPath) {
    return false;
}

bool AssetDatabase::IsValidFolder(std::string path) {
    return false;
}

std::string AssetDatabase::GenerateUniqueAssetPath(std::string path) {
    return std::string();
}

std::string AssetDatabase::GetAssetOrScenePath(Object *assetObject) {
    return std::string();
}

std::string AssetDatabase::GetAssetPath(Object *assetObject) {
    return std::string();
}

std::string AssetDatabase::GUIDToAssetPath(Unity::GUID guid) {
    return std::string();
}

void AssetDatabase::ImportAsset(std::string path, ImportAssetOptions options) {

}

TPtr<Object> AssetDatabase::LoadAssetAtPath(std::string assetPath, std::type_info type) {
    return TPtr<Object>(nullptr);
}

TPtr<Object> AssetDatabase::LoadMainAssetAtPath(std::string assetPath) {
    return TPtr<Object>(nullptr);
}

std::string AssetDatabase::MoveAsset(std::string oldPath, std::string newPath) {
    return std::string();
}

bool AssetDatabase::CopyAsset(std::string path, std::string newPath) {
    return false;
}

bool AssetDatabase::MoveAssetToTrash(std::string path) {
    return false;
}

std::string AssetDatabase::RenameAsset(std::string pathName, std::string newName) {
    return std::string();
}

void AssetDatabase::SaveAssets() {

}

bool AssetDatabase::OpenAsset(Object *target) {
    std::string s = GetAssetPath(target);
    if (s.empty())
        return false;
    try {
#if defined(_WIN32)
        fs::path p = s;
        p = fs::absolute(p);

        ShellExecuteW(0, 0, p.generic_wstring().data(), 0, 0, SW_SHOW);
#elif defiend(__linux__)
        if(fork() == 0){
            std::system(u8"xdg-open \"" + p.generic_string() + u8"\"");
            exit(0);
        }else{
            return true;
        }
#endif
    } catch (...) {
        return false;
    }
    return false;
}

void AssetDatabase::Refresh(ImportAssetOptions options) {

}

void AssetDatabase::CreateAsset(Object *asset, std::string path) {

}

bool AssetDatabase::DeleteAsset(std::string path) {
    return false;
}

Unity::GUID AssetDatabase::CreateFolder(std::string parentFolder, std::string newFolderName) {
    return Unity::GUID();
}
