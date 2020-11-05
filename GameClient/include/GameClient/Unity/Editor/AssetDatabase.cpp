//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "AssetDatabase.h"
#include "EditorUtility.h"
#include <map>
#include <filesystem>
#include <chrono>
#include <GameApi/StringFormatter.h>
#include <GameApi/GlobalLogSource.h>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <set>
#include <execution>
#include <GameClient/Unity/Editor/OneGuidFile.h>

#if defined(_WIN32)

#include <windows.h>
#include <GameClient/Unity/Serialization/JsonSerializer.h>

#elif defined(__linux__)

#include <sys/types.h>
#include <unistd.h>

#endif

namespace fs = std::filesystem;

struct Data {
    Data() = default;

    Data(const Data &) = delete;

    Data &operator=(const Data &) = delete;

    std::map<Unity::GUID, OneGUIDFile> objects;
    ///TODO: DO path to guid map

    std::map<std::string, std::vector<std::string>> dir_tree;
};

static Data &get_data() {
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
    std::unordered_set<std::string> guids;

    auto g = AssetPathToGUID(pathName);

    std::function<void(Unity::GUID)> rec = [&](Unity::GUID guid) {
        if (guid.empty()) {
            return;
        }

        auto it = d.objects.find(guid);
        if (it != d.objects.end()) {
            guids.emplace(it->second.path); //depends on itself
            for (auto &ik : it->second.dependency) {
                auto[it, cons] = guids.emplace(ik);
                if (cons && recursive) {
                    rec(AssetPathToGUID(ik));
                }
            }
        }
    };

    rec(g);

    std::vector<std::string> result;
    for (auto &ik : guids) {
        result.push_back(ik);
    }

    return result;
}

TPtr<Texture2D> AssetDatabase::GetCachedIcon(std::string path) {
    return TPtr<Texture2D>(nullptr);
}

bool AssetDatabase::TryGetGUIDAndLocalFileIdentifier(TPtr<Object> obj, Unity::GUID &guid, Unity::fileID &localId) {
    const auto &d = get_data();

    std::mutex m;
    std::find_if(std::execution::par_unseq, d.objects.begin(), d.objects.end(), [&, obj](auto &&ob) {
        auto found = std::find_if(ob.second.object.begin(), ob.second.object.end(), [&obj](auto &&ob) {
            if (ob.second == obj) {
                return true;
            }
            return false;
        });
        if (found != ob.second.object.end()) {
            std::lock_guard<std::mutex> guard(m);
            guid = ob.first;
            localId = found->first;
            return true;
        }
        return false;
    });
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
    auto result = GetAssetPath(assetObject);
    if (result.empty()) {
        ///result = Scene path
    }
    return result;
}

std::string AssetDatabase::GetAssetPath(Object *assetObject) {
    const auto &d = get_data();
    for (auto &it : d.objects) {
        auto p = std::find_if(it.second.object.begin(), it.second.object.end(),
                              [&](auto &p) { return p.second.get() == assetObject; });
        if (p != it.second.object.end()) {
            return it.second.path;
        }
    }
    return {};
}

std::string AssetDatabase::GUIDToAssetPath(Unity::GUID guid) {
    auto &d = get_data();
    auto it = d.objects.find(guid);

    if (it != d.objects.end()) {
        return it->second.path;
    } else {
        return {};
    }
}

Unity::GUID AssetDatabase::AssetPathToGUID(std::string path) {
    const auto &d = get_data();
    auto it = std::find_if(d.objects.begin(), d.objects.end(),
                           [&path](const auto &i) { return i.second.path == path; });
    if (it != d.objects.end()) {
        return it->first;
    } else {
        return {};
    }
}

void AssetDatabase::ImportAsset(std::string path, ImportAssetOptions options) {
    static thread_local std::unordered_set<std::string> working;
    auto[it, b] = working.emplace(path);
    //TODO: Create scope exit class
    try {

        if (!b) {
            GameApi::log(ERR.fmt("Circle in asset loading: %s", path.data()));
            return;
        }

    } catch (...) {
        working.erase(it);
        throw;
    }
    working.erase(it);
}

TPtr<Object> AssetDatabase::LoadAssetAtPath(std::string assetPath, std::type_info type) {
    return TPtr<Object>(nullptr);
}

TPtr<Object> AssetDatabase::LoadMainAssetAtPath(std::string assetPath) {
    return TPtr<Object>(nullptr);
}

std::string AssetDatabase::MoveAsset(std::string oldPath, std::string newPath) {
    return std::string("false");
}

bool AssetDatabase::CopyAsset(std::string path, std::string newPath) {
    return false;
}

bool AssetDatabase::MoveAssetToTrash(std::string path) {
    return false;
}

std::string AssetDatabase::RenameAsset(std::string pathName, std::string newName) {
    fs::path p = pathName;
    p.replace_filename(newName);
    return MoveAsset(pathName, p.generic_string());
}

void AssetDatabase::SaveAsset(OneGUIDFile *o) {
    if (!o->context) {
        o->context = std::shared_ptr<AssetImportContext>(new AssetImportContext(o));
        o->importer->importSettingsMissing = true;
    } else {
        o->importer->importSettingsMissing = false;
    }
    o->context->assetPath = o->path;
    o->importer->m_assetPath = o->path;

    if (o->importer->importSettingsMissing) {
        try {
            o->importer->OnExportAsset(o->context);
        } EXCEPTION_PRINT
    }

    try {
        JsonSerializer j;
        auto result = j.Serialize(o->importer.get());

        if (!GameApi::saveFullFile(o->path + ".meta", result.dump())) {
            GameApi::log(ERR.fmt("Can't save object: %s",
                                 (o->path + ".meta").data()));
        } else {
            std::for_each(o->object.begin(), o->object.end(), [](auto &&ob) {
                EditorUtility::ClearDirty(ob.second);
            });
        }

    } EXCEPTION_PRINT
}

void AssetDatabase::SaveAssets() {
    auto &d = get_data();
    std::for_each(d.objects.begin(), d.objects.end(), [](auto &&it) {
        if (it.second.main && it.second.importer) {
            if (!std::any_of(it.second.object.begin(), it.second.object.end(), [](auto &&o) {
                return EditorUtility::IsDirty(o.second);
            })) { return; }

            SaveAsset(&it.second);
        }
    });
}

bool AssetDatabase::OpenAsset(Object *target) {
    std::string s = GetAssetPath(target);
    if (s.empty())
        return false;
    try {
        fs::path p = s;
        p = fs::absolute(p);
#if defined(_WIN32)

        ShellExecuteW(0, 0, p.generic_wstring().data(), 0, 0, SW_SHOW);

#elif defined(__linux__)

        if (fork() == 0) {
            std::system(("xdg-open \"" + p.generic_string() + "\"").data());
            exit(0);
        } else {
            return true;
        }

#endif
    }
    catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
        return false;
    }
    return true;
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

            mutable int64_t priority = INT64_MIN;
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

        /// Erase deleted assets
        for (auto iterator = d.objects.begin(); iterator != d.objects.end();) {
            /// path don't exists (deleted files)
            if (files.find(iterator->second.path) == files.end()) {
                //remove dependences
                std::for_each(std::execution::par_unseq, d.objects.begin(), d.objects.end(), [&iterator](auto &&f) {
                    f.second.dependency.erase(iterator->second.path);
                });
                //remove loaded objects

                std::for_each(std::execution::par_unseq, iterator->second.object.begin(), iterator->second.object.end(),
                              [](auto &&f) {
                                  Object::DestroyImmediate(f.second.get(), true);
                              });

                iterator = d.objects.erase(iterator);
            } else {
                ++iterator;
            }
        }

        /// Erase loaded valid assets
        for (auto iterator = files.begin(); iterator != files.end();) {
            bool er = [&]() {
                auto guid = AssetPathToGUID(iterator->first);
                if (!guid.empty()) {
                    auto &ob = d.objects[guid];
                    if (ob.meta_time == iterator->second.meta && ob.asset_time == iterator->second.filename) {
                        return true;
                    }
                }
                return false;
            }();
            if (er) {
                iterator = files.erase(iterator);
            } else {
                ++iterator;
            }
        }

        auto setCompare = [](auto &&f1, auto &&f2) {
            if (f1.second.priority == INT64_MIN) {
                f1.second.priority = 0;
                try {
                    f1.second.priority = MetaData::get_importer(f1.second.extension).second;
                }
                EXCEPTION_PRINT

            }
            if (f2.second.priority == INT64_MIN) {
                f2.second.priority = 0;
                try {
                    f2.second.priority = MetaData::get_importer(f2.second.extension).second;
                }
                EXCEPTION_PRINT
            }

            return f1.second.priority < f2.second.priority;
        };
        std::set<std::pair<std::string, A>, decltype(setCompare)> sorted(files.begin(), files.end(), setCompare);

        for (auto &f : sorted) {
            ImportAsset(f.first, ImportAssetOptions::ForceUpdate);
        }

    }
    EXCEPTION_PRINT

}

void AssetDatabase::CreateAsset(Object *asset, std::string path) {

}

bool AssetDatabase::DeleteAsset(std::string path) {
    return false;
}

Unity::GUID AssetDatabase::CreateFolder(std::string parentFolder, std::string newFolderName) {
    return Unity::GUID();
}
