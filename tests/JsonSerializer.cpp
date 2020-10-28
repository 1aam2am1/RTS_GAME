//
// Created by Michal_Marszalek on 28.10.2020.
//

#define TEST

#include <GameClient/Unity/Editor/ObjectFactory.h>
#include <GameClient/Unity/Macro.h>
#include <GameClient/Unity/Serialization/JsonSerializer.h>

#undef WARN
#undef INFO

#include "catch.hpp"


class JSONTestObject : public Object {
public:
    int64_t x{};
    int64_t y{};
    std::string str;
    double d{};
};

EXPORT_CLASS(JSONTestObject, x, y, str, d)

TEST_CASE("JSON Serialization / Deserialization") {

    SECTION("To string") {
        JSONTestObject o;

        o.x = 6;
        o.y = 15;
        o.str = "\"It should work!@#$%^&*(";
        o.d = 999.99;

        JsonSerializer j;
        auto str = j.Serialize(&o).dump(-1, ' ', true);

        REQUIRE(str ==
                "{\"JSONTestObject\":{\"d\":999.99,\"name\":\"\",\"str\":\"\\\"It should work!@#$%^&*(\",\"x\":6,\"y\":15}}");
    }

    SECTION("From string") {

        JsonSerializer j;
        auto o = j.Deserialize("JSONTestObject",
                               "{\"d\":999.99,\"name\":\"Name\",\"str\":\"\\\"It should work!@#$%^&*(\",\"x\":6,\"y\":15}"_json);

        TPtr<JSONTestObject> t{nullptr};

        t = dynamic_pointer_cast<JSONTestObject>(o);

        REQUIRE(t.get());
        REQUIRE(t->name == "Name");
        REQUIRE(t->x == 6);
        REQUIRE(t->y == 15);
        REQUIRE((999 < t->d && t->d < 1000));
        REQUIRE(t->str == "\"It should work!@#$%^&*(");
    }
}
