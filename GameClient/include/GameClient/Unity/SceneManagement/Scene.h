//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_SCENE_H
#define RTS_GAME_SCENE_H


#include <string>
#include <vector>
#include <memory>
#include <GameClient/Unity/Core/GameObject.h>
#include <GameClient/TPtr.h>

class Scene {
public:
    /// Return the index of the Scene in the Build Settings.
    /// \note A Scene that is not added to the Scenes in Build window returns a buildIndex one more than the highest in
    /// the list. For example, if you don’t add a Scene to a Scenes in Build window that already has 6
    /// Scenes in it, then Scene.buildIndex returns 6 as its index.
    /// \note If the Scene is loaded through an AssetBundle, Scene.buildIndex returns -1.
    const int buildIndex;

    /// Returns true if the Scene is modified.
    const bool isDirty;

    /// Returns true if the Scene is loaded.
    const bool isLoaded;

    /// Whether this is a valid Scene.
    /// \details A Scene may be invalid if, for example, you tried to open a Scene that does not exist.
    /// In this case, the Scene returned from EditorSceneManager.OpenScene would return False for IsValid.
    const bool isValid;

    /// Returns the name of the Scene that is currently active in the game or app.
    /// \note The name limits to 244 characters. The Scene name defaults to scene.
    std::string name;

    /// Returns the relative path of the Scene. Like: "Assets/MyScenes/MyScene.unity".
    const std::string_view path;

    /// Returns all the root game objects in the Scene.
    const std::vector<TPtr<GameObject>> &getRootGameObjects() const noexcept;

    constexpr bool operator==(const Scene &r) const noexcept {
        return this == &r;
    }

    constexpr bool operator!=(const Scene &r) const noexcept {
        return !(*this == r);
    }
};


#endif //RTS_GAME_SCENE_H
