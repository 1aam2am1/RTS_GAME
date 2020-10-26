//
// Created by Michal_Marszalek on 26.10.2020.
//

#ifndef RTS_GAME_EDITORUTILITY_H
#define RTS_GAME_EDITORUTILITY_H

#include <Core/Object.h>
#include <GameClient/TPtr.h>

class EditorUtility {
public:

    /// Marks target object as dirty. (Only suitable for non-scene objects).
    static void SetDirty(TPtr<Object> target);

    /// Clear target's dirty flag.
    static void ClearDirty(TPtr<Object> target);

    /// Gets a boolean value that indicates whether the specified object has changed since the last time it was saved.
    /// \return True if the object has changed; otherwise false.
    static bool IsDirty(TPtr<Object> target);

    /// Unloads assets that are not used.
    static void UnloadUnusedAssetsImmediate();
};


#endif //RTS_GAME_EDITORUTILITY_H
