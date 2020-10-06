//
// Created by Michal_Marszalek on 28.09.2020.
//

#include "AssetDatabase.h"
#include <map>
#include <filesystem>
#include <chrono>
#include <GameApi/StringFormatter.h>
#include <GameApi/GlobalLogSource.h>

namespace fs = std::filesystem;

struct D {
    std::vector<std::string> folders{};
    std::vector<std::string> files{};

    std::chrono::time_point<std::chrono::steady_clock> creation = std::chrono::steady_clock::now();
};

const D &dir(std::string str) {
    //TODO: Remove static map
    static std::map<fs::path, D> map;

    fs::path path = "./Assets/" + (str.size() ? str : ".");

    if (!fs::is_directory(path)) {
        throw std::runtime_error(std::string{"Path is not a directory: "} + path.generic_string());
    }

    auto it = fs::directory_iterator(path, fs::directory_options::skip_permission_denied);

    D &result = map[path];
    if (std::chrono::steady_clock::now() - result.creation < std::chrono::seconds(1)) { return result; }
    result.creation = std::chrono::steady_clock::now();
    result.folders.clear();
    result.files.clear();

    auto d = DBG;
    bool error = false;
    for (const auto &i : it) {
        if (i.is_directory()) {
            result.folders.emplace_back(i.path().filename().string());
        } else if (i.is_regular_file()) {
            result.files.emplace_back(i.path().filename().string());
        } else {
            d.fmt("Object: %s not directory or file. Not supported", i.path().generic_string().c_str());
            error = true;
        }
    }
    if (error) { GameApi::log(d); }

    return result;
}

const std::vector<std::string> &AssetDatabase::GetSubFolders(std::string path) {
    return dir(path).folders;
}

const std::vector<std::string> &AssetDatabase::GetSubFiles(std::string path) {
    return dir(path).files;
}
