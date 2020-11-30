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

#define FUNC_META_PAIR(TYPE) std::pair<std::function<void(TYPE)>, std::function<TYPE(void)>>

TEST_CASE("Object Factory Serialization") {
    TestObject o;

    SECTION("default") {
        auto reflections = MetaData::getReflection(&o);

        REQUIRE(reflections.getFields.size() == 5);
    }

    SECTION("Good export") {
        int i_count = 0;
        int d_count = 0;
        int s_count = 0;
        int object_count = 0;
        int some_else = 0;
        auto reflections = MetaData::getReflection(&o);
        for (auto it: reflections.getFields) {
            auto vi = overload{[&](FUNC_META_PAIR(int64_t)) { ++i_count; },
                               [&](FUNC_META_PAIR(double)) { ++d_count; },
                               [&](FUNC_META_PAIR(std::string)) { ++s_count; },
                               [&](FUNC_META_PAIR(TPtr<Object>)) { ++object_count; },
                               [&](auto &&) { ++some_else; }};

            std::visit(vi, it.second);
        }

        REQUIRE(i_count == 2);
        REQUIRE(d_count == 1);
        REQUIRE(s_count == 2);
        REQUIRE(object_count == 0);
        REQUIRE(some_else == 0);
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

        auto reflections = MetaData::getReflection(&o);
        for (auto it: reflections.getFields) {
            auto vi = overload{[&](FUNC_META_PAIR(int64_t) p) {
                if (it.first == "x") {
                    REQUIRE(p.second() == o.x);
                } else if (it.first == "y") {
                    REQUIRE(p.second() == o.y);
                } else {
                    REQUIRE(0);
                }
            },
                               [&](FUNC_META_PAIR(double) p) {
                                   if (it.first == "d") {
                                       REQUIRE(p.second() == o.d);
                                   } else {
                                       REQUIRE(0);
                                   }
                               },
                               [&](FUNC_META_PAIR(std::string) p) {
                                   if (it.first == "str") {
                                       REQUIRE(p.second() == o.str);
                                   } else if (it.first == "m_Name") {
                                       REQUIRE(p.second() == o.name);
                                   } else {
                                       REQUIRE(0);
                                   }
                               },
                               [&](FUNC_META_PAIR(TPtr<Object>)) {
                                   REQUIRE(0);
                               },
                               [&](auto &&) {
                                   REQUIRE(0);
                               }};

            std::visit(vi, it.second);
        }
    }

    SECTION("Create from name") {
        auto con = MetaData::getReflection(typeid(TestObject));

        REQUIRE(con.name == "TestObject");

        auto p = con.CreateInstance();
        REQUIRE(dynamic_cast<TestObject *>(p.get()));
        auto &so = *p.get();
        REQUIRE(typeid(so) == typeid(TestObject));
    }

    SECTION("Type from name") {
        REQUIRE_NOTHROW(MetaData::getReflection("TestObject"));
        auto it = MetaData::getReflection("TestObject");
        REQUIRE(it.type == typeid(TestObject));
    }


}

#pragma GCC diagnostic pop
