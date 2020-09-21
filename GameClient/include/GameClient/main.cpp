#include <SFML/Graphics.hpp>
#include <GameClient/Argv_options.h>
#include <GameApi/GlobalLogSource.h>
#include <Version/version.h>
#include <imgui-SFML.h>
#include <imgui.h>

int main(int argc, char **argv) {
    try {
        Argv_options options;
        sf::RenderWindow window;

        options.process(argc, argv);

        if (options.getOptions().version) {
            GameApi::log(INFO.fmt("Version: %s %i bit\n", (Version::GIT_TAG + " " +
                                                           Version::GIT_SHA + " " +
                                                           Version::DATE).c_str(),
                                  sizeof(void *) * 8));
        }

        window.create(sf::VideoMode{options.getOptions().size.x, options.getOptions().size.y}, "RTS_GAME",
                      sf::Style::Default, sf::ContextSettings{0, 0, 0, 2, 1});
        ImGui::SFML::Init(window);

        sf::Clock deltaClock;
        while (window.isOpen()) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::ShowDemoWindow();


            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }

    }
    catch (const std::exception &e) {
        GameApi::log(ERR.fmt("%s", e.what()));
    }

    GameApi::log(INFO << "Bye");

    return 0;
}
