//
// Created by Michal_Marszalek on 12.10.2020.
//

#include "catch.hpp"
#include <Yaml/Emitter.h>

TEST_CASE("EmitterTest") {
    Emitter e;

    SECTION("String") {
        Node n;
        n = "String";
        e << n;
        REQUIRE(e.message() == "String");
    }

    SECTION("String escape") {
        Node n;
        n = "\" \\ / \n \t ";
        e << n;
        REQUIRE(e.message() == "\"\\\" \\\\ / \\n \\t \"");
    }

    SECTION("Map") {
        Node m;
        m["empty"];
        m["a"] = "b";
        e << m;
        REQUIRE(e.message() == "empty: \na: b");
    }

    SECTION("Sequence") {
        Node m;
        m[0] = 1;
        m[1] = 2;
        m[2] = 3;
        e << m;
        REQUIRE(e.message() == "- 1\n- 2\n- 3");
    }

    SECTION("Map") {
        Node m;
        m["empty"];
        m["a"] = "b";
        m.style() = EmitterStyle::Flow;
        e << m;
        REQUIRE(e.message() == "{empty: , a: b}");
    }

    SECTION("Sequence") {
        Node m;
        m[0] = 1;
        m[1] = 2;
        m[2] = 3;
        m.style() = EmitterStyle::Flow;
        e << m;
        REQUIRE(e.message() == "[1, 2, 3]");
    }

}
