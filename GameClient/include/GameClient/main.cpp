#include <SFML/Graphics.hpp>
#include <GameClient/Argv_options.h>
#include <GameApi/GlobalLogSource.h>
#include <Version/version.h>
#include <GameClient/Unity/Editor/Menu.h>
#include "MainWindow.h"
#include "FontLoader.h"
#include <filesystem>
#include <GameClient/Windows/AssetWindow.h>
#include <GameClient/Unity/Editor/AssetDatabase.h>

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

        MainWindow window(options);
        FontLoader{options};

#if UNITY_EDITOR
        fs::create_directory("Assets");
        Menu::Init();
        AssetWindow::Init(); //=>To Coroutine
#else
        if(!std::filesystem::exists("Assets")){throw std::runtime_error("Assets folder don't exists")};
#endif
        AssetDatabase::Refresh();

        window.run();
    }
    catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
    }

    GameApi::log(INFO << "Bye");

    return 0;
}
