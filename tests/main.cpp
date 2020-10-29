//
// Created by Michal_Marszalek on 08.09.2020.
//
#define CATCH_CONFIG_RUNNER

#include <GameClient/Initializer.h>

#undef WARN
#undef INFO

#include "catch.hpp"

int main(int argc, char *argv[]) {
    Initializer::initialize();
    return Catch::Session().run(argc, argv);
}
