//
// Created by Michal_Marszalek on 23.09.2020.
//

#ifndef RTS_GAME_EDITORWINDOW_H
#define RTS_GAME_EDITORWINDOW_H


#include <GameClient/Unity/Core/ScriptableObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <GameApi/SetterGetter.h>
#include <list>
#include <typeindex>

class MainWindow;

/// Derive from this class to create an editor window.
/// \details Create your own custom editor window that can float free or be docked as a tab,
/// just like the native windows in the Unity interface.
class EditorWindow : public ScriptableObject {
public:

    /// The EditorWindow which currently has keyboard focus. (Read Only)
    static EditorWindow *focusedWindow();

    /// The EditorWindow currently under the mouse cursor. (Read Only)
    static EditorWindow *mouseOverWindow();

    /// The maximum size of this window.
    sf::Vector2f maxSize = {4000, 4000};

    /// The minimum size of this window.
    sf::Vector2f minSize = {100, 100};

    /// The desired position of the window in screen space.
    /// \attention Setting this value will undock the window if it is docked.
    SetterGetter<sf::FloatRect> position;

    /// The GUIContent used for drawing the title of EditorWindows.
    std::string titleContent;

    /// Close the editor window.
    void Close();

    /// Moves keyboard focus to another EditorWindow.
    void Focus();

    /// Stop showing notification message.
    void RemoveNotification();

    /// Show the EditorWindow window.
    void Show();

    /// Shows a window with dropdown behaviour and styling.
    void ShowAsDropDown(sf::FloatRect buttonRect, sf::Vector2f windowSize);

    /// Show the EditorWindow as a floating modal window.
    /// \details Not dockable
    void ShowUtility();

    /// Show a notification message.
    /// \param notification The contents of the notification message.
    /// \param fadeoutWait The duration the notification is displayed. Measured in seconds.
    void ShowNotification(std::function<void()> notification, std::chrono::seconds fadeoutWait);

    /// Shows an Editor window using popup-style framing.
    void ShowPopup();

    /// Creates an EditorWindow of type T.
    /// \tparam T The type of the window. Must derive from EditorWindow.
    /// \param title The title of the created window. If this value is null, use the class name as title.
    /// \param desiredDockNextTo An array of EditorWindow types that the window will attempt to dock onto.
    template<typename T>
    static std::shared_ptr<T> CreateWindow(std::string title, std::list<std::type_index> desiredDockNextTo = {});

    /// Focuses the first found EditorWindow of specified type if it is open.
    /// \param t The type of the window. Must derive from EditorWindow.
    static void FocusWindowIfItsOpen(std::type_index t);

    /// Returns the first EditorWindow of type t which is currently on the screen.
    /// \tparam T The type of the window. Must derive from EditorWindow.
    /// \param title If GetWindow creates a new window, it will get this title.
    /// If this value is null, use the class name as title.
    /// \param focus Whether to give the window focus, if it already exists.
    /// (If GetWindow creates a new window, it will always get focus).
    template<typename T>
    static EditorWindow GetWindow(std::string title = {}, bool focus = true);

    /// Returns the first EditorWindow of type t which is currently on the screen.
    /// \tparam T The type of the window. Must derive from EditorWindow.
    /// \param rect The position on the screen where a newly created window will show.
    /// \param title If GetWindow creates a new window, it will get this title.
    /// If this value is null, use the class name as title.
    template<typename T>
    static EditorWindow GetWindowWithRect(sf::FloatRect rect, std::string title = {});

    /// Checks if an editor window is open.
    /// \param t The type of the window. Must derive from EditorWindow.
    /// \return Returns true if an EditorWindow, matching the specified type, is open. Returns false otherwise.
    static bool HasOpenInstances(std::type_index t);

    /// Called when the window gets keyboard focus.
    virtual void OnFocus() {}

    /// Implement your own editor GUI here.
    virtual void OnGUI() {}

    /// Called when the window loses keyboard focus.
    virtual void OnLostFocus() {}

    /// Called multiple times per second on all visible windows.
    virtual void Update() {}

private:
    friend class MainWindow;

    void drawGui();
};


#endif //RTS_GAME_EDITORWINDOW_H
