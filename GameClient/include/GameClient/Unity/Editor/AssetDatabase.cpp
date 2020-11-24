//
// Created by Michal_Marszalek on 28.09.2020.
//

#if defined(_WIN32)

#include <windows.h>

#elif defined(__linux__)

#include <sys/types.h>
#include <unistd.h>

#endif

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
#include <GameClient/Unity/Serialization/JsonSerializer.h>
#include <queue>
#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/Editor/SceneAsset.h>

namespace fs = std::filesystem;

struct FileTime {
    Unity::GUID guid{};

    fs::file_time_type asset_time{};
    fs::file_time_type meta_time{};

    uint64_t md5_asset = 0;
    uint64_t md5_time = 0;

    bool operator==(const FileTime &) = delete;

    bool operator!=(const FileTime &) = delete;
};

struct Data {
    Data() = default;

    Data(const Data &) = delete;

    Data &operator=(const Data &) = delete;

    std::map<Unity::GUID, OneGUIDFile> objects;

    std::map<std::string, FileTime> path_files;

    std::map<std::string, std::set<std::string>> dir_tree;
};

static Data &get_data() {
    static Data d;
    return d;
}

namespace {
    static int UNIQUE_ID(p) = Initializer::d_add([]() {
        auto &d = get_data();

        d.path_files.clear();
        d.dir_tree.clear();

        for (auto iterator: d.objects) {
            std::for_each(std::execution::par_unseq, iterator.second.object.begin(), iterator.second.object.end(),
                          [](auto &&f) {
                              Object::DestroyImmediate(f.second.get(), true);
                          });
        }
    });
}

void SaveAsset(Unity::GUID g, OneGUIDFile *o);

std::pair<Unity::GUID, OneGUIDFile>
ImportAssetGlobal(std::string assetPath, ImportAssetOptions options) {
    //TODO: Loop loading of assets
    //TODO: Load only loaded, or new??? <- SMALL
    auto &d = get_data();

    auto &file = d.path_files[assetPath];

    auto status = fs::directory_entry(assetPath);
    auto meta_status = fs::directory_entry(assetPath + ".meta");

    //asset don't exists
    if (!fs::exists(status)) {
        AssetDatabase::DeleteAsset(assetPath);
        return {};
    }

    if (options == ImportAssetOptions::Default &&
        fs::exists(meta_status) &&
        file.meta_time == meta_status.last_write_time() &&
        file.asset_time == status.last_write_time()) {

        auto guid = AssetDatabase::AssetPathToGUID(assetPath);
        assert(!guid.empty());
        auto it = d.objects.find(guid);
        if (it != d.objects.end()) { return *it; }
    }

    nlohmann::json meta_j;

    if (fs::exists(meta_status)) {
        auto str = GameApi::readFullFile(assetPath + ".meta");
        meta_j = nlohmann::json::parse(str.empty() ? "{}" : str);
        if (meta_j["guid"].empty()) {
            GameApi::log(ERR.fmt("No guid in loaded meta data: %s", (assetPath + ".meta").data()));
        }
    }
    if (!file.guid.empty() && file.guid != meta_j["guid"].get<Unity::GUID>()) {
        GameApi::log(ERR.fmt("Changed guid of file between loadings %s", (assetPath + ".meta").data()));
        //TODO: Remove not valid asset
        // What should be done when moved this asset elsewhere and moved another in this place. And this file was first?
        ///AssetDatabase::DeleteAsset(file.guid);
    }
    //TODO: Check if guid is from another path therefore check if there
    // exists meta with this same guid and if true change this guid

    file.guid = meta_j["guid"].get<Unity::GUID>();

    bool new_guid = false;
    if (file.guid.empty()) {
        file.guid = Unity::GUID::NewGuid();
        new_guid = true;
    }


    std::string extension;
    if (status.is_regular_file()) {
        fs::path path = assetPath;
        extension = path.extension().generic_string();
    } else if (status.is_directory()) {
        fs::path path = assetPath;
        auto parent = path.parent_path().relative_path().string();

        d.dir_tree[parent].emplace(path.filename().string());
        extension = "_UNITY/D";
    } else {
        auto e = ERR;
        e.fmt("Can't load object %s of this type: ", assetPath.data());

        if (fs::is_regular_file(status)) e << " is a regular file\n";
        if (fs::is_directory(status)) e << " is a directory\n";
        if (fs::is_block_file(status)) e << " is a block device\n";
        if (fs::is_character_file(status)) e << " is a character device\n";
        if (fs::is_fifo(status)) e << " is a named IPC pipe\n";
        if (fs::is_socket(status)) e << " is a named IPC socket\n";
        if (fs::is_symlink(status)) e << " is a symlink\n";
        if (!fs::exists(status)) e << " does not exist\n";
        GameApi::log(e);
        return {};
    }
    try {
        OneGUIDFile result;
        {
            result.path = assetPath;

            auto[type, priority] = MetaData::get_importer(extension);
            auto[name, constructor] = MetaData::get_name_constructor(type);
            nlohmann::json object = meta_j[name.data()];
            result.importer = dynamic_pointer_cast<AssetImporter>(constructor->create());

            result.importer->assetPath = assetPath;
            result.importer->importSettingsMissing = object.empty();

            if (!result.importer->importSettingsMissing) {
                JsonSerializer js;
                js.Deserialize(result.importer, object);
            }

            AssetImportContext context(&result);
            context.assetPath = assetPath;

            try {
                result.importer->OnImportAsset(context);
            } catch (const std::exception &e) {
                GameApi::log(ERR.fmt("%s", e.what()));
                return {};
            }

            if (new_guid || object.empty()) {
                SaveAsset(file.guid, &result);
            } else {
                file.meta_time = meta_status.last_write_time();
                file.asset_time = status.last_write_time();
            }
        }

        //replace old assets and return
        d.objects[file.guid] = result;
        return std::make_pair(file.guid, result);

    }
    EXCEPTION_PRINT

    return {};
}

std::vector<std::string> AssetDatabase::GetSubFolders(std::string path) {
    std::vector<std::string> result;
    const auto it = get_data().dir_tree.find(path);
    if (it != get_data().dir_tree.end()) {

        result.reserve(it->second.size());
        for (auto &s: it->second) {
            result.emplace_back(s);
        }
    }
    return result;
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

/*
TPtr<Texture2D> AssetDatabase::GetCachedIcon(std::string path) {
    return TPtr<Texture2D>(nullptr);
}
*/
bool AssetDatabase::TryGetGUIDAndLocalFileIdentifier(TPtr<Object> obj, Unity::GUID &guid, Unity::fileID &localId) {
    const auto &d = get_data();

    std::mutex m;
    auto it = std::find_if(std::execution::par_unseq, d.objects.begin(), d.objects.end(), [&, obj](auto &&ob) {
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

    std::lock_guard<std::mutex> guard(m);
    return it != d.objects.end();
}

std::vector<TPtr<Object>> AssetDatabase::LoadAllAssetsAtPath(std::string assetPath) {
    auto one = ImportAssetGlobal(assetPath, ImportAssetOptions::Default);
    if (one.first.empty()) { return {}; }
    std::vector<TPtr<Object>> copy;
    copy.reserve(one.second.object.size());
    for (auto &it: one.second.object) {
        copy.push_back(it.second);
    }

    return copy;
}

std::vector<TPtr<Object>> AssetDatabase::LoadAllAssetRepresentationsAtPath(std::string assetPath) {
    auto one = ImportAssetGlobal(assetPath, ImportAssetOptions::Default);
    if (one.first.empty()) { return {}; }
    std::vector<TPtr<Object>> copy;
    copy.reserve(one.second.object.size());
    for (auto &it: one.second.object) {
        if (it.second != one.second.main) { copy.push_back(it.second); }
    }

    return copy;
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

/*
std::string AssetDatabase::GetAssetOrScenePath(Object *assetObject) {
    auto result = GetAssetPath(assetObject);
    if (result.empty()) {
        ///result = Scene path
    }
    return result;
}
*/
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

Unity::GUID AssetDatabase::AssetPathToGUID(std::string_view path) {
    const auto &d = get_data();
    /*auto it = std::find_if(d.objects.begin(), d.objects.end(),
                           [&path](const auto &i) { return i.second.path == path; });
    if (it != d.objects.end()) {
        return it->first;
    } else {
        return {};
    }*/
    auto it = d.path_files.find(std::string{path});
    if (it != d.path_files.end()) {
        return it->second.guid;
    } else {
        return {};
    }
}


void AssetDatabase::ImportAsset(std::string assetPath, ImportAssetOptions options) {
    ImportAssetGlobal(assetPath, options);
}

TPtr<Object> AssetDatabase::LoadAssetAtPath(std::string assetPath, std::type_info type) {
    auto one = ImportAssetGlobal(assetPath, ImportAssetOptions::Default);
    if (one.first.empty()) { return TPtr<Object>(nullptr); }

    auto it = std::find_if(one.second.object.begin(), one.second.object.end(), [&](auto &&it) {
        return typeid(*it.second.get()) == type;
    });

    if (it != one.second.object.end()) {
        return it->second;
    } else {
        return TPtr<Object>(nullptr);
    }
}

TPtr<Object> AssetDatabase::LoadMainAssetAtPath(std::string assetPath) {
    auto one = ImportAssetGlobal(assetPath, ImportAssetOptions::Default);
    if (one.first.empty()) { return TPtr<Object>(nullptr); }

    return one.second.main;
}

/*
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
*/
void SaveAsset(Unity::GUID g, OneGUIDFile *o) {
    auto &d = get_data();

    AssetImportContext context(o);
    o->importer->importSettingsMissing = !fs::directory_entry(o->path).exists();

    context.assetPath = o->path;
    o->importer->assetPath = o->path;

    try {
        o->importer->OnExportAsset(context);
    }
    EXCEPTION_PRINT

    try {
        JsonSerializer j;
        auto result = j.Serialize(o->importer.get());

        result["guid"] = g;

        if (!GameApi::saveFullFile(o->path + ".meta", result.dump(2, ' ', true))) {
            GameApi::log(ERR.fmt("Can't save object: %s",
                                 (o->path + ".meta").data()));
        } else {
            for (auto &ob: o->object) {
                EditorUtility::ClearDirty(ob.second);
            }
        }

    }
    EXCEPTION_PRINT

    auto &files = d.path_files[o->path];
    files.guid = g;
    files.asset_time = fs::last_write_time(o->path);
    files.meta_time = fs::last_write_time(o->path + ".meta");
}

void AssetDatabase::SaveAssets() {
    auto &d = get_data();
    for (auto &it : d.objects) {
        if (it.second.main && it.second.importer) {
            if (!std::any_of(it.second.object.begin(), it.second.object.end(), [](auto &&o) {
                return EditorUtility::IsDirty(o.second);
            })) { return; }

            SaveAsset(it.first, &it.second);
        }
    }
}

bool AssetDatabase::OpenAsset(TPtr<Object> target) {
    std::string s = GetAssetPath(target.get());
    if (s.empty())
        return false;

    //open types in our unity if they can be opened
    //TODO: Make map of types and opening function
    if (typeid(*target.get()) == typeid(SceneAsset)) {
        SceneManager::LoadScene(s, SceneManager::LoadSceneMode::Single);
        return true;
    }

    try {
        fs::path p = s;
        p = fs::absolute(p);
#if defined(_WIN32)

        if (((INT) (UINT_PTR) ShellExecuteW(0, 0, p.generic_wstring().data(), 0, 0, SW_SHOW)) > 31) {
            return true;
        } else {
            return false;
        }

#elif defined(__linux__)

        int status = std::system(("xdg-open \"" + p.generic_string() + "\" &").data());
        if (status < 0) { return false; }
        if (!WIFEXITED(status)) { return false; }

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
        struct A : FileTime {
            std::string extension{};

            mutable int64_t priority = INT64_MIN;

            Unity::GUID guid;
            OneGUIDFile backup;
        };

        std::unordered_map<std::string, A> files;

        /// Check all files and mark as have meta or not.
        for (auto &p :fs::recursive_directory_iterator("Assets", fs::directory_options::skip_permission_denied)) {
            auto name = p.path().relative_path().string();
            auto extension = p.path().extension().string();

            if (p.is_regular_file()) {
                if (extension == ".meta") {
                    name.erase(name.size() - 5, std::string::npos); //Remove .meta

                    files[name].meta_time = p.last_write_time();
                } else {
                    files[name].asset_time = p.last_write_time();
                    files[name].extension = extension;
                }
            } else if (p.is_directory()) {
                if (extension == ".meta") {
                    name.erase(name.size() - 5, std::string::npos); //Remove .meta

                    files[name].meta_time = p.last_write_time();
                } else {
                    files[name].asset_time = p.last_write_time();;
                }

                files[name].extension = "_UNITY/D";

                auto parent = p.path().parent_path().relative_path().string();

                d.dir_tree[parent].emplace(p.path().filename().string());
            }
        }
//TODO: Only when options is Default
        /// Erase loaded valid assets (not changed) and not valid extensions
        for (auto iterator = files.begin(); iterator != files.end();) {
            auto ob = d.path_files.find(iterator->first);

            ///check if can import
            if (!MetaData::exists_importer(iterator->second.extension)) {
                GameApi::log(ERR.fmt("File %s with extension %s don't have mapped object importer: use EXPORT_IMPORTER",
                                     iterator->first.data(), iterator->second.extension.data()));
                iterator = files.erase(iterator);
                continue;
            }

            if (ob != d.path_files.end() &&
                ob->second.meta_time == iterator->second.meta_time &&
                ob->second.asset_time == iterator->second.asset_time) {
                //TODO: Load files md5 and check

                iterator->second.guid = ob->second.guid;
                iterator->second.backup = d.objects[ob->second.guid];

                d.objects.erase(ob->second.guid);
                d.path_files.erase(ob);
            }

            iterator->second.priority = MetaData::get_importer(iterator->second.extension).second;
            ++iterator;
        }

        std::map<Unity::GUID, OneGUIDFile> old_connections = d.objects;
        d.objects.clear();

        auto setCompare = [](auto &&f1, auto &&f2) {
            return f1.second.priority > f2.second.priority;
        };
        using PAIR = std::pair<std::string, A>;
        std::priority_queue<PAIR, std::vector<PAIR>, decltype(setCompare)> sorted(files.begin(), files.end(),
                                                                                  setCompare);

        ///import assets
        while (!sorted.empty()) {
            auto &ob = sorted.top();
            if (ob.second.guid.empty()) {
                ImportAssetGlobal(ob.first, options);
            } else {
                d.objects[ob.second.guid] = ob.second.backup;
                d.path_files[ob.first] = ob.second;
            }
            sorted.pop();
        }

        /// Erase deleted assets, move non deleted
        for (auto iterator = old_connections.begin(); iterator != old_connections.end(); ++iterator) {
            auto new_object = d.objects.find(iterator->first);
            if (new_object == d.objects.end()) {
                //remove dependences
                ///TODO: Remove asset!!!
                std::for_each(std::execution::par_unseq, d.objects.begin(), d.objects.end(),
                              [&iterator](auto &&f) {
                                  f.second.dependency.erase(iterator->second.path);
                              });
                std::for_each(std::execution::par_unseq, old_connections.begin(), old_connections.end(),
                              [&iterator](auto &&f) {
                                  f.second.dependency.erase(iterator->second.path);
                              });
                //remove loaded objects

                std::for_each(std::execution::par_unseq, iterator->second.object.begin(), iterator->second.object.end(),
                              [](auto &&f) {
                                  Object::DestroyImmediate(f.second.get(), true);
                              });
            } else {
                //Resets old TPtr to new values
                iterator->second = new_object->second;
            }
        }
    }
    EXCEPTION_PRINT

}

void AssetDatabase::CreateAsset(Object *asset, std::string path) {
    auto &d = get_data();
    Unity::GUID g;
    std::map<Unity::GUID, OneGUIDFile>::iterator it;
    bool b;
    fs::path p = path;
    ///TODO: Check if in Assets...

    DeleteAsset(path);

    do {
        g = Unity::GUID::NewGuid();
        auto[it2, b2] = d.objects.emplace(g, OneGUIDFile());
        it = it2;
        b = b2;
    } while (!b);

    it->second.path = path;
    it->second.main = Object::Instantiate(asset);
    it->second.object.emplace(0, it->second.main);

    it->second.importer = MetaData::get_name_constructor(
            MetaData::get_importer(p.extension().generic_string()).first).second->create();

    SaveAsset(g, &it->second);
}

bool AssetDatabase::DeleteAsset(std::string path) {
    auto &d = get_data();

    auto guid = AssetPathToGUID(path);

    auto iterator = d.objects.find(guid);

    bool removed = fs::remove(path);
    if (!removed) { return false; }
    fs::remove(path + ".meta");

    std::for_each(std::execution::par_unseq, d.objects.begin(), d.objects.end(), [&iterator](auto &&f) {
        f.second.dependency.erase(iterator->second.path);
    });
    //remove loaded objects

    if (iterator != d.objects.end()) {
        std::for_each(std::execution::par_unseq, iterator->second.object.begin(), iterator->second.object.end(),
                      [](auto &&f) {
                          Object::DestroyImmediate(f.second.get(), true);
                      });

        d.objects.erase(iterator);
    }

    d.path_files.erase(path);
    {
        fs::path p = path;
        auto parent = p.parent_path().relative_path().string();

        auto f = d.dir_tree.find(parent);
        if (f != d.dir_tree.end()) {
            f->second.erase(p.filename().string());
        }
    }

    return true;
}

Unity::GUID AssetDatabase::CreateFolder(std::string parentFolder, std::string newFolderName) {
    auto assetPath = parentFolder + "/" + newFolderName;
    if (fs::create_directory(assetPath)) {
        auto[guid, result] = ImportAssetGlobal(assetPath, ImportAssetOptions::ForceUpdate);
        return guid;
    } else {
        return {};
    }
}

