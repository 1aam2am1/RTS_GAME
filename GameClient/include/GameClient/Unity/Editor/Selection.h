//
// Created by Michal_Marszalek on 15.10.2020.
//

#ifndef RTS_GAME_SELECTION_H
#define RTS_GAME_SELECTION_H

#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/Unity/Core/Guid.h>
#include <vector>
#include <GameClient/TPtr.h>

class Selection {
public:
    /// Returns the active game object. (The one shown in the inspector).
    /// \details It will also return game objects that might be Prefabs or non-modifiable objects.
    static TPtr<GameObject> activeGameObject;

    /// Returns the actual object selection. Includes Prefabs, non-modifiable objects.
    static TPtr<Object> activeObject;

    /// Returns the active transform. (The one shown in the inspector).
    /// \details This will never return Prefabs or non-modifiyable objects.
    static TPtr<Transform> activeTransform;

    /// Returns the guids of the selected assets.
    static std::vector<Unity::GUID> assetGUIDs;
};


#endif //RTS_GAME_SELECTION_H
