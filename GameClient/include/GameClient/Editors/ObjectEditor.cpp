//
// Created by Michal_Marszalek on 18.11.2020.
//

#include <GameClient/Unity/Editor/Editor.h>
#include <GameClient/Unity/Macro.h>

class ObjectEditor : public Editor {
public:
    void OnInspectorGUI() override {
        DrawDefaultInspector();
    }
};

CUSTOM_EDITOR_FALLBACK(ObjectEditor, Object, true);
