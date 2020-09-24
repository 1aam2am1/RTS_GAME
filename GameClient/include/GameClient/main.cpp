#include <SFML/Graphics.hpp>
#include <GameClient/Argv_options.h>
#include <GameApi/GlobalLogSource.h>
#include <Version/version.h>
#include "MainWindow.h"

int main(int argc, char **argv) {
    try {
        Argv_options options;

        options.process(argc, argv);

        if (options.getOptions().version) {
            GameApi::log(INFO.fmt("Version: %s %i bit\n", (Version::GIT_TAG + " " +
                                                           Version::GIT_SHA + " " +
                                                           Version::DATE).c_str(),
                                  sizeof(void *) * 8));
        }

        MainWindow window(options);

#if UNITY_EDITOR

#else

#endif

        window.run();
    }
    catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
    }

    GameApi::log(INFO << "Bye");

    return 0;
}
