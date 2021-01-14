//
// Created by Michal_Marszalek on 24.09.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"
#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Editor/Selection.h>
#include <GameClient/WindowLayout.h>
#include <Editor/AssetDatabase.h>

class Inspector : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Inspector>();
        WindowLayout::dockWindow(WindowLayout::Right, window);
        window->Show();
    }

    TPtr<Object> activeObject;
    TPtr<Editor> editor;

    void Awake() override {
        minSize.x = 250.f;
    }

    void Update() override {
        if (Application::isPlaying() && AssetDatabase::Contains(Selection::activeObject.get())) {
            editor = nullptr;
        } else if (Selection::activeObject != activeObject) {
            activeObject = Selection::activeObject;

            editor = nullptr;
            if (activeObject) Editor::CreateCachedEditor(activeObject, editor);
        }
    }

    void OnDrawGizmos() {
        if (editor) { editor->OnSceneGUI(); }
    }

    void OnGUI() override {
        if (editor) { editor->OnInspectorGUI(); }

        /// TODO: Move OnDrawGizmos as override?
        OnDrawGizmos();
    }
};

MENU_ITEM(Inspector::Init, "Window/General/Inspector", 4);

#if UNITY_EDITOR
INITIALIZE_FUNC(Inspector::Init());
#endif
