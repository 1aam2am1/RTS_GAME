//
// Created by Michal_Marszalek on 10.09.2020.
//
#include <GameApi/Signal.h>
#include "catch.hpp"

TEST_CASE("Signal") {

    sigslot::signal<> s;

    struct A {
        void e() {
            i += 1;
        }

        int i = 0;
    };

    A a;

    SECTION("add") {
        s.connect(&A::e, &a);
        REQUIRE(s.slot_count() == 1);
    }

    SECTION("execution") {
        s.connect(&A::e, &a);
        s();
        REQUIRE(a.i == 1);
    }

    SECTION("remove") {
        s.connect(&A::e, &a);
        s.disconnect(&A::e, &a);
        REQUIRE(s.slot_count() == 0);
    }
}
