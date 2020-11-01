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
    int64_t x{};
    int64_t y{};
    std::string str;
    double d{};
};

EXPORT_CLASS(TestObject, x, y, str, d)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

TEST_CASE("Object Factory Serialization") {
    TestObject o;

    SECTION("default") {
        auto reflections = MetaData::get_reflections(&o);

        REQUIRE(reflections.size() == 2);
    }

    SECTION("Good export") {
        int i_count = 0;
        int d_count = 0;
        int s_count = 0;
        int object_count = 0;
        int some_else = 0;
        auto reflections = MetaData::get_reflections(&o);
        for (auto &it: reflections) {
            for (auto &w: it->get(&o)) {
                auto vi = overload{[&](int64_t *) { ++i_count; },
                                   [&](double *) { ++d_count; },
                                   [&](std::string *) { ++s_count; },
                                   [&](TPtr<Object> *) { ++object_count; },
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
        auto reflections = MetaData::get_reflections(&o);
        for (auto &it: reflections) {
            for (auto &w: it->get(&o)) {
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
                                       } else if (w.first == "m_Name") {
                                           REQUIRE(s == &o.name);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](Object *) {
                                       REQUIRE(0);
                                   },
                                   [&](auto &&) {
                                       REQUIRE(0);
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

        auto reflections = MetaData::get_reflections(&o);
        for (auto &it: reflections) {
            for (auto &w: it->get(&o)) {
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
                                       } else if (w.first == "m_Name") {
                                           REQUIRE(*s == o.name);
                                       } else {
                                           REQUIRE(0);
                                       }
                                   },
                                   [&](Object *) {
                                       REQUIRE(0);
                                   },
                                   [&](auto &&) {
                                       REQUIRE(0);
                                   }};

                std::visit(vi, w.second);
            }
        }
    }

    SECTION("Create from name") {
        auto con = MetaData::get_name_constructor(typeid(TestObject));

        REQUIRE(con.first == "TestObject");

        auto p = con.second->create();
        REQUIRE(dynamic_cast<TestObject *>(p.get()));
        auto &so = *p.get();
        REQUIRE(typeid(so) == typeid(TestObject));
    }

    SECTION("Type from name") {
        REQUIRE_NOTHROW(MetaData::get_type("TestObject"));
        auto it = MetaData::get_type("TestObject");
        REQUIRE(it == typeid(TestObject));
    }


}

#pragma GCC diagnostic pop
