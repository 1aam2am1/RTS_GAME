//
// Created by Michal_Marszalek on 25.11.2020.
//

#ifndef RTS_GAME_EDITORSCENEMANAGER_H
#define RTS_GAME_EDITORSCENEMANAGER_H

#include <GameClient/Unity/SceneManagement/SceneManager.h>
#include <GameClient/Unity/Editor/SceneAsset.h>
#include <unordered_set>


class EditorSceneManager : public SceneManager {
public:

    enum class NewSceneSetup {
        EmptyScene, ///< No game objects are added to the new Scene.
        DefaultGameObjects, ///< Adds default game objects to the new Scene (a light and camera).
    };

    enum class OpenSceneMode {
        Single, ///< Closes all current loaded Scenes and loads a Scene. Use DontDestroyOnLoad to keep game objects
        Addictive, ///< Adds the Scene to the current loaded Scenes. Display both.
        AdditiveWithoutLoading ///<	Adds a Scene to the current open Scenes without loading it. It will show up as 'unloaded' in the Hierarchy Window.
    };

    /// Loads this SceneAsset when you start Play Mode.
    static TPtr<SceneAsset> playModeStartScene;

    /// Close the Scene. If removeScene flag is true, the closed Scene will also be removed from EditorSceneManager.
    /// \param scene The Scene to be closed/removed.
    /// \param removeScene Bool flag to indicate if the Scene should be removed after closing.
    static bool CloseScene(SceneManager::SceneP scene, bool removeScene);

    /// Mark the specified Scene as modified.
    static bool MarkSceneDirty(SceneManager::SceneP scene);

    /// Create a new Scene.
    static SceneManager::SceneP
    NewScene(NewSceneSetup setup, SceneManager::LoadSceneMode mode = SceneManager::LoadSceneMode::Single);

    /// Open a Scene in the Editor.
    static SceneManager::SceneP OpenScene(std::string_view scenePath, OpenSceneMode mode = OpenSceneMode::Single);

    /// Save a Scene.
    /// \param scene The Scene to be saved.
    /// \param dstScenePath The file path to save the Scene to. If the path is empty, the current open Scene is overwritten.
    /// If it has not yet been saved at all, a save dialog is shown.
    /// \param saveAsCopy If set to true, the Scene is saved without changing the current Scene,
    /// and without clearing the unsaved changes marker.
    static bool SaveScene(SceneManager::SceneP scene, std::string_view dstScenePath = "", bool saveAsCopy = false);

    /// Save all open Scenes.
    /// \return Returns true if all open Scenes are successfully saved.
    static bool SaveOpenScenes();
};


#endif //RTS_GAME_EDITORSCENEMANAGER_H
