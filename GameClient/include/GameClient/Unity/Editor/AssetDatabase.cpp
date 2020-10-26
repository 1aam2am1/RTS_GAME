//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "AssetDatabase.h"
#include <map>
#include <filesystem>
#include <chrono>
#include <GameApi/StringFormatter.h>
#include <GameApi/GlobalLogSource.h>
#include <unordered_set>

#if defined(_WIN32)

#include <windows.h>

#endif

namespace fs = std::filesystem;
using fileID = uint64_t;

struct D {
    std::map<fileID, TPtr<Object>> object{}; //fileID -> objects in this file

    TPtr<Object> main{nullptr}; //main object of this file

    std::time_t asset_time = 0;
    std::time_t meta_time = 0;
};

struct Data {
    Data() = default;

    Data(const Data &) = delete;

    std::map<Unity::GUID, D> objects;
    std::map<Unity::GUID, std::string> paths;

    ///TODO: Change dependency such as prefab is dependant of all children prefabs
    std::map<Unity::GUID, std::vector<Unity::GUID>> dependency; //root->have many dependency assets

    std::map<std::string, std::vector<std::string>> dir_tree;
};

Data &get_data() {
    static Data d;
    return d;
}

std::vector<std::string> AssetDatabase::GetSubFolders(std::string path) {
    const auto it = get_data().dir_tree.find(path);
    if (it != get_data().dir_tree.end()) {
        return it->second;
    }
    return {};
}

std::vector<std::string> AssetDatabase::GetDependencies(std::string pathName, bool recursive) {
    const auto &d = get_data();
    std::unordered_set<Unity::GUID> guids;

    auto g = AssetPathToGUID(pathName);

    std::function<void(Unity::GUID)> rec = [&](Unity::GUID guid) {
        if (guid.empty()) {
            return;
        }

        auto it = d.dependency.find(guid);
        if (it != d.dependency.end()) {
            for (auto &ik : it->second) {
                auto[it, cons] = guids.emplace(ik);
                if (cons && recursive) {
                    rec(ik);
                }
            }
        }
    };

    rec(g);

    std::vector<std::string> result;
    for (auto &ik : guids) {
        auto str = GUIDToAssetPath(ik);
        if (!str.empty()) {
            result.push_back(str);
        }
    }

    return result;
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
    const auto &d = get_data();
    for (auto &it : d.objects) {
        auto p = std::find_if(it.second.object.begin(), it.second.object.end(),
                              [&](auto &p) { return p.second.get() == obj; });
        if (p != it.second.object.end()) { return it.second.main.get() != obj; }
    }
    return false;
}

bool AssetDatabase::IsMainAsset(Object *obj) {
    const auto &d = get_data();
    auto p = std::find_if(d.objects.begin(), d.objects.end(),
                          [&](auto &p) { return p.second.main.get() == obj; });
    if (p != d.objects.end()) { return true; }
    return false;
}

bool AssetDatabase::Contains(Object *obj) {
    const auto &d = get_data();
    for (auto &it : d.objects) {
        auto p = std::find_if(it.second.object.begin(), it.second.object.end(),
                              [&](auto &p) { return p.second.get() == obj; });
        if (p != it.second.object.end()) { return true; }
    }
    return false;
}

bool AssetDatabase::IsMainAssetAtPathLoaded(std::string assetPath) {
    const auto &d = get_data();
    auto guid = AssetPathToGUID(assetPath);
    auto it = d.objects.find(guid);
    if (it != d.objects.end()) {
        return it->second.main.get() != nullptr;
    }
    return false;
}

bool AssetDatabase::IsValidFolder(std::string path) {
    const auto &d = get_data();
    return d.dir_tree.find(path) != d.dir_tree.end();
}

std::string AssetDatabase::GenerateUniqueAssetPath(std::string path) {
    fs::path p(path);
    p = p.lexically_normal();
    if (p.string().substr(0, 7) != "Assets/") {
        p = fs::path("Assets") / p.string();
    }
    if (p.string().substr(0, 7) != "Assets/") { return {}; }

    auto status = fs::status(p);
    if (!fs::exists(status)) { return path; }

    auto name = p.stem().string();
    auto extension = p.extension().string();
    auto str = GameApi::rsubstr(name, ' ');
    int i = 0;
    if (GameApi::is_int(str)) {
        i = GameApi::to_int(str);
    }

    while (i < 1000) {
        p.replace_filename(name + ' ' + GameApi::to_string(++i) + extension);
        status = fs::status(p);
        if (!fs::exists(status)) { return path; }
    }

    return {};
}

std::string AssetDatabase::GetAssetOrScenePath(Object *assetObject) {
    return std::string();
}

std::string AssetDatabase::GetAssetPath(Object *assetObject) {
    const auto &d = get_data();
    for (auto &it : d.objects) {
        auto p = std::find_if(it.second.object.begin(), it.second.object.end(),
                              [&](auto &p) { return p.second.get() == assetObject; });
        if (p != it.second.object.end()) {
            auto it2 = d.paths.find(it.first);
            if (it2 != d.paths.end()) {
                return it2->second;
            }
            break;
        }
    }
    return {};
}

std::string AssetDatabase::GUIDToAssetPath(Unity::GUID guid) {
    auto &d = get_data();
    auto it = d.paths.find(guid);

    if (it != d.paths.end()) {
        return it->second;
    } else {
        return {};
    }
}

Unity::GUID AssetDatabase::AssetPathToGUID(std::string path) {
    auto &d = get_data();
    auto it = std::find_if(d.paths.begin(), d.paths.end(), [&path](const auto &i) { return i.second == path; });
    if (it != d.paths.end()) {
        return it->first;
    } else {
        return {};
    }
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
    auto &d = get_data();
    d.dir_tree.clear();

    try {
        auto it = fs::recursive_directory_iterator("Assets", fs::directory_options::skip_permission_denied);

        struct A {
            std::time_t meta = 0;
            std::time_t filename = 0;
            std::string extension{};
        };

        std::map<std::string, A> files;

        /// Check all files and mark as have meta or not.
        for (auto &p :it) {
            auto name = p.path().relative_path().string();
            auto extension = p.path().extension().string();
            auto ftime = std::chrono::duration_cast<std::chrono::seconds>(
                    p.last_write_time().time_since_epoch()).count();

            if (p.is_regular_file()) {
                if (extension == ".meta") {
                    name.erase(name.size() - 5, std::string::npos); //Remove .meta

                    files[name].meta = ftime;
                } else {
                    files[name].filename = ftime;
                    files[name].extension = extension;
                }
            } else if (p.is_directory()) {
                if (extension == ".meta") {
                    name.erase(name.size() - 5, std::string::npos); //Remove .meta

                    files[name].meta = ftime;
                } else {
                    files[name].filename = ftime;
                }

                files[name].extension = "_UNITY/D";

                auto parent = p.path().parent_path().relative_path().string();

                d.dir_tree[parent].push_back(p.path().filename().string());
            }
        }

        for (auto &f : files) {
            auto guid = AssetPathToGUID(f.first);
            /// guid -> exists meta to -> check time
            /// guid -> no meta yes -> load meta

            /// guid -> exists meta not -> new meta new guid
            /// guid -> no meta no -> new meta guid

            /// meta -> TextureImport, ..., DefaultImport...
            /// import -> run
            /// import -> meta file as default
            /// objects -> path file as default
            if (!guid.empty()) {
                auto &ob = d.objects[guid];
                if (ob.meta_time != f.second.meta || ob.asset_time != f.second.filename) {
                    ///TODO: Load add to new container guid, and if in old container serialize data into old one
                }
            } else {
                if (f.second.meta != 0) {
                    ///TODO: Load and add to container
                } else {
                    ///TODO: Create default meta file and try loading and save meta
                }
            }
        }

        ///TODO: Change dir_tree to new database
        ///TODO: Move objects, delete unused, delete deleted.

    } catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
    }
}

void AssetDatabase::CreateAsset(Object *asset, std::string path) {

}

bool AssetDatabase::DeleteAsset(std::string path) {
    return false;
}

Unity::GUID AssetDatabase::CreateFolder(std::string parentFolder, std::string newFolderName) {
    return Unity::GUID();
}
