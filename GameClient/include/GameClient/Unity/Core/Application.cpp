//
// Created by Michal_Marszalek on 24.11.2020.
//

#include <Editor/EditorApplication.h>
#include "Application.h"

#if UNITY_EDITOR

bool Application::isPlaying() {
    return EditorApplication::isPlaying;
}

#endif
