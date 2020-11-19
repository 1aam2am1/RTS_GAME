//
// Created by Michal_Marszalek on 17.11.2020.
//

#ifndef RTS_GAME_EDITOR_H
#define RTS_GAME_EDITOR_H


#include <GameClient/Unity/Core/ScriptableObject.h>
#include <GameClient/TPtr.h>
#include <typeindex>
#include <map>

class Editor : public ScriptableObject {
public:

    /// A SerializedObject representing the object or objects being inspected.
    /// Use the serializedObject inside the OnInspectorGUI function of a custom Editor,
    /// as described on the page about the Editor class.
    /// Do not use the serializedObject inside OnSceneGUI or OnPreviewGUI.
    /// Use the target property directly in those callback functions instead.
    ///TPtr<SerializedObject> serializedObject{this};

    /// The object being inspected.
    TPtr<Object> target{this};

    /// Enables the Editor to handle an event in the Scene view.
    /// Draw on Scene
    virtual void OnSceneGUI();

    /// Implement this function to make a custom inspector.
    virtual void OnInspectorGUI() = 0;

    /// Draws the built-in inspector.
    bool DrawDefaultInspector();

    virtual std::string GetPreviewTitle();

    /// Draws the inspector GUI with a foldout header for target.
    /// \param target The object to display the Inspector for.
    /// \param editor The reference to a variable of type Editor.
    static void DrawFoldoutInspector(TPtr<Object> target, TPtr<Editor> &editor);

    /// Make a custom editor for targetObject or targetObjects.
    /// \details By default, an appropriate editor is created that has a matching CustomEditor attribute.
    /// If an editorType is specified, an editor of that type is created instead. Use this if you have
    /// created multiple custom editors where each editor shows different properties of the object.
    /// Returns NULL if no approprate editor was found.
    static TPtr<Editor> CreateEditor(TPtr<Object> targetObject, std::type_index editorType = {typeid(nullptr)});

    /// On return previousEditor is an editor for targetObject or targetObjects. The function either returns if
    /// the editor is already tracking the objects, or destroys the previous editor and creates a new one.
    static void CreateCachedEditor(TPtr<Object> targetObject, TPtr<Editor> &previousEditor,
                                   std::type_index editorType = {typeid(nullptr)});

    /// Defines which object type the custom editor class can edit.
    /// \tparam Editor Editor class
    /// \tparam inspectedType Type that this editor can edit.
    /// \param editorForChildClasses If true, child classes of inspectedType will also show this editor. Defaults to false.
    template<typename E, typename inspectedType>
    static void addCustom(bool editorForChildClasses = false);

    /// Defines which object type the custom editor class can edit. Normal then fallback
    /// \tparam Editor Editor class
    /// \tparam inspectedType Type that this editor can edit.
    /// \param editorForChildClasses If true, child classes of inspectedType will also show this editor. Defaults to false.
    template<typename E, typename inspectedType>
    static void addCustomFallback(bool editorForChildClasses = false);

private:
    struct Data {
        std::function<TPtr<Editor>()> create{};
        bool editorForChildClasses{};
        std::function<bool(TPtr<Object>)> check_child{};
    };

    static std::map<std::type_index, const Data> t_editor;
    static std::map<std::type_index, const Data> t_fallback_editor;
};


template<typename E, typename inspectedType>
void Editor::addCustom(bool editorForChildClasses) {
    Data result;
    result.create = []() { return TPtr<Editor>{nullptr, std::make_shared<E>()}; };
    result.editorForChildClasses = editorForChildClasses;
    result.check_child = [](auto &&e) { return !dynamic_pointer_cast<inspectedType>(e).expired(); };
    auto[it, b] = t_editor.emplace(typeid(inspectedType), std::move(result));

    if (!b) {
        GameApi::log(ERR.fmt("Declared second editor to the same inspected type %s %s %s",
                             GameApi::demangle(it->first.name()).data(),
                             GameApi::demangle(typeid(E).name()).data(),
                             GameApi::demangle(typeid(inspectedType).name()).data()));
    }
}


template<typename E, typename inspectedType>
void Editor::addCustomFallback(bool editorForChildClasses) {
    Data result;
    result.create = []() { return TPtr<Editor>{nullptr, std::make_shared<E>()}; };
    result.editorForChildClasses = editorForChildClasses;
    result.check_child = [](auto &&e) { return !dynamic_pointer_cast<inspectedType>(e).expired(); };
    auto[it, b] = t_fallback_editor.emplace(typeid(inspectedType), std::move(result));

    if (!b) {
        GameApi::log(ERR.fmt("Declared second fallback editor to the same inspected type %s %s %s",
                             GameApi::demangle(it->first.name()).data(),
                             GameApi::demangle(typeid(E).name()).data(),
                             GameApi::demangle(typeid(inspectedType).name()).data()));
    }
}


#endif //RTS_GAME_EDITOR_H
