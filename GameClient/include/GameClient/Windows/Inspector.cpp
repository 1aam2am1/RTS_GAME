//
// Created by Michal_Marszalek on 24.09.2020.
//

#include <GameClient/Unity/Editor/EditorWindow.h>
#include "Macro.h"
#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Editor/Selection.h>

class Inspector : public EditorWindow {
public:

    static void Init() {
        // Get existing open window or if none, make a new one:
        auto window = EditorWindow::GetWindow<Inspector>();
        window->Show();
    }

    TPtr<Object> activeObject{this};
    TPtr<Editor> editor{this};

    void Update() override {
        if (Selection::activeObject != activeObject) {
            activeObject = Selection::activeObject;

            editor = nullptr;
            Editor::CreateCachedEditor(activeObject, editor);
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
