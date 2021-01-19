//
// Created by Michal_Marszalek on 08.09.2020.
//

#ifndef RTS_GAME_SCENEMANAGER_H
#define RTS_GAME_SCENEMANAGER_H

#include <SceneManagement/Scene.h>
#include <string_view>
#include <GameClient/Unity/Yield/AsyncOperation.h>
#include <GameClient/TPtr.h>

class GameObject;

/// Scene management at run-time.
class SceneManager {
public:

    using SceneP = TPtr<Scene>;

    /// Used when loading a Scene in a player.
    /// \details Use LoadSceneMode to choose what type of Scene loads when using SceneManager.LoadScene.
    /// The available modes are Single and Additive.
    /// \details Single mode loads a standard Unity Scene which then appears on its own in the Hierarchy window. Additive loads a Scene which appears in the Hierarchy window while another is active.
    enum class LoadSceneMode {
        Single, ///< Closes all current loaded Scenes and loads a Scene. Use DontDestroyOnLoad to keep game objects
        Addictive ///< Adds the Scene to the current loaded Scenes. Display both.
    };

    /// The total number of currently loaded Scenes.
    /// \details The number of currently loaded Scenes will be returned.
    static int sceneCount();

    /// Number of Scenes in Build Settings.
    /// \details The number of Scenes which have been added to the Build Settings.
    static uint32_t sceneCountInBuildSettings();

    /// Create an empty new Scene at runtime with the given name.
    /// \details The new Scene will be opened additively into the hierarchy alongside any existing Scenes that are currently open.
    /// The path of the new Scene will be empty. This function is for creating Scenes at runtime.
    /// To create a Scene at edit-time inherits from Scene and add to buildSettings.
    /// \param sceneName The name of the new Scene. It cannot be empty or null, or same as the name of the existing Scenes.
    /// \return A reference to the new Scene that was created, or an invalid Scene if creation failed.
    static SceneP createScene(std::string_view sceneName);

    /// Gets the currently active Scene.
    /// \details The currently active Scene is the Scene which will be used as the target for new GameObjects
    /// instantiated by scripts.
    /// \return The active Scene.
    static SceneP GetActiveScene();

    /// Get a Scene struct from a build index.
    /// \details This method will return a valid Scene if a Scene has been added to the build settings
    /// at the given build index AND the Scene is loaded. If it has not been loaded yet the SceneManager
    /// cannot return a valid Scene.
    /// \param buildIndex Build index as shown in the Build Settings.
    /// \return A reference to the Scene, if valid. If not, an invalid Scene is returned.
    static SceneP GetSceneByBuildIndex(int buildIndex);

    /// Searches through the Scenes loaded for a Scene with the given name.
    /// \param name Name of Scene to find.
    /// \return A reference to the Scene, if valid. If not, an invalid Scene is returned.
    static SceneP GetSceneByName(std::string_view name);

    /// Get the Scene at index in the SceneManager's list of loaded Scenes.
    /// \param Index of the Scene to get. Index must be greater than or equal to 0 and less than SceneManager.sceneCount.
    static SceneP GetSceneAt(int index);

    /// Loads the Scene by its name or index in Build Settings.
    /// \note  In most cases, to avoid pauses or performance hiccups while loading,
    /// you should use the asynchronous version of this command which is: LoadSceneAsync.
    /// \details When using SceneManager.LoadScene, the scene loads in the next frame, that is it does not load immediately.
    /// This semi-asynchronous behavior can cause frame stuttering and can be confusing because load does not complete immediately.
    /// \details Because loading is set to complete in the next rendered frame, calling SceneManager.LoadScene forces all
    /// previous AsyncOperations to complete, even if AsyncOperation.allowSceneActivation is set to false.
    /// To avoid this, use LoadSceneAsync instead.
    /// \param sceneBuildIndex Index of the Scene in the Build Settings to load.
    /// \param mode Allows you to specify whether or not to load the Scene additively.
    /// See LoadSceneMode for more information about the options.
    static void LoadScene(int sceneBuildIndex, LoadSceneMode mode = LoadSceneMode::Single);

    /// Loads the Scene by its name or index in Build Settings.
    /// \note  In most cases, to avoid pauses or performance hiccups while loading,
    /// you should use the asynchronous version of this command which is: LoadSceneAsync.
    /// \details When using SceneManager.LoadScene, the scene loads in the next frame, that is it does not load immediately.
    /// This semi-asynchronous behavior can cause frame stuttering and can be confusing because load does not complete immediately.
    /// \details Because loading is set to complete in the next rendered frame, calling SceneManager.LoadScene forces all
    /// previous AsyncOperations to complete, even if AsyncOperation.allowSceneActivation is set to false.
    /// To avoid this, use LoadSceneAsync instead.
    /// \param sceneName Name or path of the Scene to load.
    /// \param mode Allows you to specify whether or not to load the Scene additively.
    /// See LoadSceneMode for more information about the options.
    static void LoadScene(std::string_view sceneName, LoadSceneMode mode = LoadSceneMode::Single);

    /// Loads the Scene asynchronously in the background
    /// \param sceneBuildIndex Index of the Scene in the Build Settings to load.
    /// \param mode If LoadSceneMode.Single then all current Scenes will be unloaded before loading.
    /// \return AsyncOperation Use the AsyncOperation to determine if the operation has completed.
    static AsyncOperation LoadSceneAsync(int sceneBuildIndex, LoadSceneMode mode = LoadSceneMode::Single);

    /// Loads the Scene asynchronously in the background
    /// \param sceneName Name or path of the Scene to load.
    /// \param mode If LoadSceneMode.Single then all current Scenes will be unloaded before loading.
    /// \return AsyncOperation Use the AsyncOperation to determine if the operation has completed.
    static AsyncOperation LoadSceneAsync(std::string_view sceneName, LoadSceneMode mode = LoadSceneMode::Single);

    /// This will merge the source Scene into the destinationScene.
    /// \details This function merges the contents of the source Scene into the destination Scene, and deletes the source Scene.
    /// All GameObjects at the root of the source Scene are moved to the root of the destination Scene.
    /// \note This function is destructive: The source Scene will be destroyed once the merge has been completed.
    /// \param sourceScene The Scene that will be merged into the destination Scene.
    /// \param destinationScene Existing Scene to merge the source Scene into.
    static void MergeScenes(SceneP sourceScene, SceneP destinationScene);

    /// Move a GameObject from its current Scene to a new Scene.
    /// \details You can only move root GameObjects from one Scene to another.
    /// This means the GameObject to move must not be a child of any other GameObject in its Scene.
    /// This only works on GameObjects being moved to a Scene that is already loaded (additive).
    /// If you want to load single Scenes, make sure to use DontDestroyOnLoad on the GameObject
    /// you would like to move to a new Scene, otherwise Unity deletes it when it loads a new Scene.
    /// \param go GameObject to move.
    /// \param scene Scene to move into.
    static void MoveGameObjectToScene(TPtr<GameObject> go, SceneP scene);

    /// Set the Scene to be active.
    /// \note There must always be one Scene marked as the active Scene.
    /// Note the active Scene has no impact on what Scenes are rendered.
    /// \param scene The Scene to be set.
    /// \return Returns false if the Scene is not loaded yet.
    static bool SetActiveScene(SceneP scene);

    /// Destroys all GameObjects associated with the given Scene and removes the Scene from the SceneManager.
    /// \note This is case-insensitive and due to it being async there are no guarantees about completion time.
    /// \note Assets are currently not unloaded. In order to free up asset memory call Resources.UnloadUnusedAssets.
    /// \note It is not possible to UnloadSceneAsync if there are no scenes to load.
    /// For example, a project that has a single scene cannot use this static member.
    /// \param scene Scene to unload.
    /// \return AsyncOperation Use the AsyncOperation to determine if the operation has completed.
    static AsyncOperation UnloadSceneAsync(SceneP scene);

    ///Subscribe to this event to get notified when the active Scene has changed.
    /// \details This script added to activeSceneChanged takes two hidden arguments.
    /// These are the replaced Scene and the next Scene. The arguments are not visible.
    /// \note Scene is unloaded before firing event.
    static sigslot::signal<SceneP, SceneP> activeSceneChanged;

    ///Add a delegate to this to get notifications when a Scene has loaded.
    static sigslot::signal<SceneP, LoadSceneMode> sceneLoaded;

    ///Add a delegate to this to get notifications when a Scene has unloaded.
    static sigslot::signal<SceneP> sceneUnloaded;

protected:
    //static bool LoadSceneFull(Data &data, std::string_view path);
};


#endif //RTS_GAME_SCENEMANAGER_H
