#include <GameClient/Argv_options.h>
#include <GameApi/GlobalLogSource.h>
#include <Version/version.h>
#include <GameClient/Unity/Editor/Menu.h>
#include "MainWindow.h"
#include "FontLoader.h"
#include "GlobalStaticVariables.h"
#include <filesystem>
#include <GameClient/Windows/AssetWindow.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>
#include <GameClient/Initializer.h>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    try {
        Argv_options options;

        options.process(argc, argv);

        if (options.getOptions().version) {
            GameApi::log(INFO.fmt("Version: %s %s %s %i bit\n", Version::GIT_TAG.data(),
                                  Version::GIT_SHA.data(),
                                  Version::DATE.data(),
                                  static_cast<int>(sizeof(void *) * 8)));
        }

        global.settings.Load();

        MainWindow window(options);
        FontLoader{options};
        Initializer::initialize();

#if UNITY_EDITOR
        fs::create_directory("Assets");
        Menu::Init();
        AssetWindow::Init(); //=>To Coroutine

#else
        if (!std::filesystem::exists("Assets")) { throw std::runtime_error("Assets folder don't exists"); }
#endif
        AssetDatabase::Refresh();
        window.run();

        Initializer::uninitialize();
    } EXCEPTION_PRINT

    GameApi::log(INFO << "Bye");

    return 0;
}
