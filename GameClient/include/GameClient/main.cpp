#include <TGUI/TGUI.hpp>
#include <GameClient/Argv_options.h>
#include <GameApi/GlobalLogSource.h>
#include <Version/version.h>

int main(int argc, char **argv) {
    try {
        Argv_options options;
        sf::RenderWindow window;
        tgui::Gui gui(window);

        options.process(argc, argv);

        if (options.getOptions().version) {
            GameApi::log(INFO.fmt("Version: %s %i bit\n", (Version::GIT_TAG + " " +
                                                           Version::GIT_SHA + " " +
                                                           Version::DATE).c_str(),
                                  sizeof(void *) * 8));
        }


    }
    catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
    }

    GameApi::log(INFO << "Bye");

    return 0;
}
