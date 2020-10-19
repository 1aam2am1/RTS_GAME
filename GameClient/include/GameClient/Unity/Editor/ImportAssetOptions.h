//
// Created by Michal_Marszalek on 19.10.2020.
//

#ifndef RTS_GAME_IMPORTASSETOPTIONS_H
#define RTS_GAME_IMPORTASSETOPTIONS_H


enum class ImportAssetOptions {
    Default, ///< Default import options.
    ForceUpdate, ///< User initiated asset import. \details This should be passed when asset import is caused by the user (e.g. by clicking Reimport). Non-user initiated import happens when the asset modification time changes.
    ForceSynchronousImport, ///< Import all assets synchronously.
};


#endif //RTS_GAME_IMPORTASSETOPTIONS_H
