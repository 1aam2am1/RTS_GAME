//
// Created by Michal_Marszalek on 10.09.2020.
//
#define TEST

#include "catch.hpp"

#undef WARN
#undef INFO

#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/MonoBehaviour.h>

TEST_CASE("TPtr") {
    class A : public MonoBehaviour {
    public:
        ~A() {
            if (d)d();
        }

        void works() {}

        std::function<void()> d;
    };

    MonoBehaviour parent;

    SECTION("CREATE nullptr parent") {
        TPtr<A> l(nullptr, std::make_shared<A>());

        REQUIRE(l);
    }

    SECTION("Create pointer with returned value") {
        TPtr<A> l(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();

        REQUIRE_NOTHROW(l->works());

        parent.onDestroySignal();

        REQUIRE_THROWS(l->works());

    }

    SECTION("Test deleting ptr and throw") {
        TPtr<A> l(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();

        REQUIRE_NOTHROW(l->works());

        l->onDestroySignal();

        REQUIRE_THROWS(l->works());
        REQUIRE(l.expired());
    }

    SECTION("TPtr destructor") {
        auto p = std::make_shared<A>();
        {
            TPtr<A> l(&parent);

            l = [=]() { return TPtr<A>(nullptr, p); }();
        }

        REQUIRE(parent.onDestroySignal.slot_count() == 0);
        REQUIRE(p->onDestroySignal.slot_count() == 0);
    }

    SECTION("Clear pointer") {
        TPtr<A> l(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();
        REQUIRE_NOTHROW(l->works());

        l = nullptr;

        REQUIRE_THROWS(l->works());
    }SECTION("Check define and destructor") {
        struct B : MonoBehaviour {
            TPTR_P(a);
            TPTR_PT(A, b);
        };

        B b;

        b.a = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();
        b.b = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();

        REQUIRE(b.onDestroySignal.slot_count() == 2);

        int i = 0;
        dynamic_cast<A *>(b.a.get())->d = [&i]() { i += 1; };
        b.b->d = [&i]() { i += 1; };

        b.onDestroySignal();

        REQUIRE(b.onDestroySignal.slot_count() == 2);
        REQUIRE(i == 2);
    }
}
