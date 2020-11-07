//
// Created by Michal_Marszalek on 02.11.2020.
//

#ifndef RTS_GAME_ONEGUIDFILE_H
#define RTS_GAME_ONEGUIDFILE_H

#include <map>
#include <GameClient/Unity/Core/Guid.h>
#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Object.h>
#include <GameClient/Unity/Editor/AssetImporter.h>

class AssetImportContext;

struct OneGUIDFile {
    /// Path of this asset file
    std::string path{};

    std::map<Unity::fileID, TPtr<Object>> object{}; //fileID -> objects in this file

    TPtr<Object> main{nullptr}; //main object of this file

    TPtr<AssetImporter> importer{nullptr};

    //root->have many dependency assets
    std::unordered_set<std::string> dependency{}; ///this object depends on...

    OneGUIDFile() = default;

    OneGUIDFile(const OneGUIDFile &) = default;

    OneGUIDFile &operator=(const OneGUIDFile &);
};


#endif //RTS_GAME_ONEGUIDFILE_H
