//
// Created by Michal_Marszalek on 10.11.2020.
//

#ifndef RTS_GAME_ASSETPREVIEW_H
#define RTS_GAME_ASSETPREVIEW_H

#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Texture2D.h>

class AssetPreview {
public:
    /// Returns a preview texture for an asset.
    static TPtr<Texture2D> GetAssetPreview(TPtr<Object> asset);

    /// Returns the thumbnail for an object (like the ones you see in the project view).
    static TPtr<Texture2D> GetMiniThumbnail(TPtr<Object> obj);
};


#endif //RTS_GAME_ASSETPREVIEW_H
