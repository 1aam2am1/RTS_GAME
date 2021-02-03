//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_SCENE_H
#define RTS_GAME_SCENE_H


#include <string>
#include <vector>
#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Object.h>
#include <GameApi/SetterGetterF.h>

class GameObject;

class Scene : public Object {
private:
    friend class SceneManager;

    friend class EditorSceneManager;

    friend class Transform;

    friend class GameObject;

    const uint64_t id;

    explicit Scene(uint64_t id);

public:
    /// Return the index of the Scene in the Build Settings.
    /// \note A Scene that is not added to the Scenes in Build window returns a buildIndex one more than the highest in
    /// the list. For example, if you don’t add a Scene to a Scenes in Build window that already has 6
    /// Scenes in it, then Scene.buildIndex returns 6 as its index.
    /// \note If the Scene is loaded through an AssetBundle, Scene.buildIndex returns -1.
    int buildIndex();

    /// Returns true if the Scene is modified.
    bool isDirty();

    /// Returns true if the Scene is loaded.
    bool isLoaded();

    /// Whether this is a valid Scene.
    /// \details A Scene may be invalid if, for example, you tried to open a Scene that does not exist.
    /// In this case, the Scene returned from EditorSceneManager.OpenScene would return False for IsValid.
    bool isValid();

    /// Returns the name of the Scene that is currently active in the game or app.
    /// \note The name limits to 244 characters. The Scene name defaults to scene.
    SetterGetterF<std::string> name;

    /// Returns the relative path of the Scene. Like: "Assets/MyScenes/MyScene.unity".
    std::string path();

    /// Returns all the root game objects in the Scene.
    [[nodiscard]] const std::vector<TPtr<GameObject>> &getRootGameObjects() const noexcept;

    bool operator==(const Scene &r) const noexcept {
        return id == r.id;
    }

    bool operator!=(const Scene &r) const noexcept {
        return !(*this == r);
    }
};


#endif //RTS_GAME_SCENE_H
