//
// Created by Michal_Marszalek on 28.09.2020.
//

#ifndef RTS_GAME_ASSETDATABASE_H
#define RTS_GAME_ASSETDATABASE_H

#include <vector>
#include <string_view>
#include <string>

class AssetDatabase {
public:

    /// Given a path to a directory in the Assets folder, relative to the project folder,
    /// this method will return an array of all its subdirectories.
    static const std::vector<std::string> &GetSubFolders(std::string path);

    ///  Given a path to a directory in the Assets folder, relative to the project folder,
    /// this method will return an array of all its files.
    /// TODO: Move it out off asset database as it don't exists in unity
    static const std::vector<std::string> &GetSubFiles(std::string path);
};


#endif //RTS_GAME_ASSETDATABASE_H
