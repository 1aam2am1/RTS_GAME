//
// Created by Michal_Marszalek on 09.09.2020.
//
#include "catch.hpp"
#include <GameClient/Argv_options.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
TEST_CASE("Argv options"){
    Argv_options o;

    SECTION("default"){
        auto options = o.getOptions();

        REQUIRE(options.size == sf::Vector2u{1200, 800});
        REQUIRE(options.version == false);
    }

    SECTION("process"){
        char n[] = "Program name";
        char  arg0[] = "-g";
        char  arg1[] = "800x600";
        char  arg2[] = "-v";
        char* argv[] = { &n[0], &arg0[0], &arg1[0], &arg2[0], NULL };
        int   argc   = (int)(sizeof(argv) / sizeof(argv[0])) - 1;

        o.process(argc, &argv[0]);
        auto options = o.getOptions();

        REQUIRE(options.size == sf::Vector2u{800, 600});
        REQUIRE(options.version == true);
    }

    SECTION("big x"){
        char n[] = "Program name";
        char  arg0[] = "-g";
        char  arg1[] = "900X700";
        char* argv[] = { &n[0], &arg0[0], &arg1[0], NULL };
        int   argc   = (int)(sizeof(argv) / sizeof(argv[0])) - 1;

        o.process(argc, &argv[0]);
        auto options = o.getOptions();

        REQUIRE(options.size == sf::Vector2u{900, 700});
    }

    SECTION("Set value"){

        o.setOptions({.size =  {100, 200},.version =  false});


        auto options = o.getOptions();

        REQUIRE(options.size == sf::Vector2u{100, 200});
        REQUIRE(options.version == false);
    }
}
#pragma GCC diagnostic pop
