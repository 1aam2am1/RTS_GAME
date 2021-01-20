//
// Created by Michal_Marszalek on 24.11.2020.
//

#include <Editor/EditorApplication.h>
#include "Application.h"

#if UNITY_EDITOR

bool Application_isPlaying = false; //Global changes when GameLoop have loop

bool Application::isPlaying() {
    return Application_isPlaying;
}

#else
#include <SFML/Graphics/RenderWindow.hpp>
#include <GameClient/GlobalStaticVariables.h>
#endif

void Application::Quit() {
#if UNITY_EDITOR
    EditorApplication::isPlaying = false;
#else
    if(global.rendering.m_window)
        global.rendering.m_window->close();
#endif
}
