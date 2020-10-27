//
// Created by Michal_Marszalek on 27.10.2020.
//
#define TEST

#include <GameClient/Unity/Editor/ObjectFactory.h>
#include <GameClient/Unity/Macro.h>
#include <GameApi/overload.h>

#undef WARN
#undef INFO

#include "catch.hpp"

class TestObject : public Object {
public:
    int64_t x;
    int64_t y;
    std::string str;
    double d;
};

EXPORT_CLASS(TestObject, x, y, str, d)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

TEST_CASE("Object Factory Serialization") {
    TestObject o;

    SECTION("default") {
        int i = 0;
        for (auto &it : ObjectFactory::reflection) {
            if (!it.second->get(&o).empty()) {
                ++i;
            }
        }

        REQUIRE(i == 2);
    }

    SECTION("Good export") {
        int i_count = 0;
        int d_count = 0;
        int s_count = 0;
        int object_count = 0;
        int some_else = 0;
        for (auto &it: ObjectFactory::reflection) {
            for (auto &w: it.second->get(&o)) {
                auto vi = overload{[&](int64_t *) { ++i_count; },
                                   [&](double *) { ++d_count; },
                                   [&](std::string *) { ++s_count; },
                                   [&](Object *) { ++object_count; },
                                   [&](auto &&) { ++some_else; }};

                std::visit(vi, w.second);
            }
        }

        REQUIRE(i_count == 2);
        REQUIRE(d_count == 1);
        REQUIRE(s_count == 2);
        REQUIRE(object_count == 0);
        REQUIRE(some_else == 0);
    }

    SECTION("Good ptr") {
        for (auto &it: ObjectFactory::reflection) {
            for (auto &w: it.second->get(&o)) {
                auto vi = overload{[&](int64_t *i) {
                    if (w.first == "x") {
                        REQUIRE(i == &o.x);
                    } else if (w.first == "y") {
                        REQUIRE(i == &o.y);
                    } else {
                        REQUIRE(0);
                    }
                },
                                   [&](double *d) {
                                       if (w.first == "d") {
                                           REQUIRE(d == &o.d);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](std::string *s) {
                                       if (w.first == "str") {
                                           REQUIRE(s == &o.str);
                                       } else if (w.first == "name") {
                                           REQUIRE(s == &o.name);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](Object *) {
                                   },
                                   [&](auto &&) {
                                   }};

                std::visit(vi, w.second);
            }
        }
    }

    SECTION("Good value") {
        TestObject v;
        v.x = 15;
        v.y = 36;
        v.d = 678.0f;
        v.str = "Some string";
        v.name = "Name";
        o = v;

        REQUIRE(o.x == 15);
        REQUIRE(o.y == 36);
        REQUIRE((o.d > 677.5f && o.d < 678.5f)); //float == is so wrong
        REQUIRE(o.str == "Some string");
        REQUIRE(o.name == "Name");

        for (auto &it: ObjectFactory::reflection) {
            for (auto &w: it.second->get(&o)) {
                auto vi = overload{[&](int64_t *i) {
                    if (w.first == "x") {
                        REQUIRE(*i == o.x);
                    } else if (w.first == "y") {
                        REQUIRE(*i == o.y);
                    } else {
                        REQUIRE(0);
                    }
                },
                                   [&](double *d) {
                                       if (w.first == "d") {
                                           REQUIRE(*d == o.d);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](std::string *s) {
                                       if (w.first == "str") {
                                           REQUIRE(*s == o.str);
                                       } else if (w.first == "name") {
                                           REQUIRE(*s == o.name);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](Object *) {
                                   },
                                   [&](auto &&) {
                                   }};

                std::visit(vi, w.second);
            }
        }
    }

}

#pragma GCC diagnostic pop
