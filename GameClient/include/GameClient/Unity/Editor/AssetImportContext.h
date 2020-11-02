//
// Created by Michal_Marszalek on 01.11.2020.
//

#ifndef RTS_GAME_ASSETIMPORTCONTEXT_H
#define RTS_GAME_ASSETIMPORTCONTEXT_H

#include <GameClient/Unity/Core/Object.h>
#include <GameClient/Unity/Core/Texture2D.h>
#include <GameClient/Unity/Macro.h>

class AssetImportContext : public Object {
public:
    using fileID = uint64_t;

    /// The path of the source asset file to be imported.
    std::string assetPath;

    /// The main object set on the AssetImportContext.
    TPTR_P(mainObject);

    /// Adds an object to the result of the import operation.
    void AddObjectToAsset(fileID identifier, TPtr<Object> obj, TPtr<Texture2D> thumbnail = TPtr<Texture2D>{nullptr});

    /// Creates dependency between the asset and a source asset.
    void DependsOnSourceAsset(std::string path);

    /// Gets the list of objects set on the AssetImportContext.
    void GetObjects(std::list<TPtr<Object>> &objects);

    /// Sets the main object for import.
    void SetMainObject(TPtr<Object> obj);
};


#endif //RTS_GAME_ASSETIMPORTCONTEXT_H
