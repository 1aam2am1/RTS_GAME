//
// Created by Michal_Marszalek on 2021-01-19.
//

#ifndef RTS_GAME_AUDIOIMPORTER_H
#define RTS_GAME_AUDIOIMPORTER_H


#include <GameClient/Unity/Editor/AssetImporter.h>
#include <GameClient/Unity/Macro.h>


class AudioImporter : public AssetImporter {
public:

    void OnImportAsset(AssetImportContext &ctx) override;
};


#endif //RTS_GAME_AUDIOIMPORTER_H
