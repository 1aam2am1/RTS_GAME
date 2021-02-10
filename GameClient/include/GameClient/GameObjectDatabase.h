//
// Created by Michal_Marszalek on 2021-02-10.
//

#ifndef RTS_GAME_GAMEOBJECTDATABASE_H
#define RTS_GAME_GAMEOBJECTDATABASE_H

#include <GameClient/TPtr.h>
#include "GuidFileIdPack.h"

class GameObjectDatabase {
public:

    /// Get the GUID and local file id from an object instance id.
    /// \return true if found
    static bool
    TryGetGUIDAndLocalFileIdentifier(const TPtr<const Object> &obj, Unity::GUID &guid, Unity::fileID &localId);

    static TPtr<> TRYGetObjectFROMGUIDAndLocalFileIdentifier(Unity::GUID &guid, Unity::fileID &localId);

    static bool Register(TPtr<const Object>, Unity::fileID);
};


#endif //RTS_GAME_GAMEOBJECTDATABASE_H
