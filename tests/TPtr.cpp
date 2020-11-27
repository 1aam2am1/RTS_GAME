//
// Created by Michal_Marszalek on 10.09.2020.
//
#define TEST

#include "catch.hpp"

#undef WARN
#undef INFO

#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Object.h>

TEST_CASE("TPtr") {
    class A : public Object {
    public:
        ~A() {
            if (d)d();
        }

        void works() {}

        std::function<void()> d;
    };

    Object parent;

    SECTION("CREATE nullptr parent") {
        TPtr<A> l(nullptr, std::make_shared<A>());

        REQUIRE(l);
    }

    SECTION("Create pointer with returned value") {
        TPtr<A> l(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();

        REQUIRE_NOTHROW(l->works());

        parent.onDestroySignal(nullptr);

        REQUIRE_THROWS(l->works());

    }

    SECTION("Test deleting ptr and throw") {
        TPtr<A> l(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();

        REQUIRE_NOTHROW(l->works());

        l->onDestroySignal(nullptr);

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
    }

    SECTION("Check define and destructor") {
        struct B : Object {
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

        b.onDestroySignal(nullptr);

        REQUIRE(b.onDestroySignal.slot_count() == 2);
        REQUIRE(i == 2);
    }

    SECTION("Operator == ") {
        TPtr<A> l(&parent);
        TPtr<A> l2(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();
        l2 = l;
        REQUIRE(l.get() == l2.get());
    }

    SECTION("Operator == Base") {
        TPtr<A> l(&parent);
        TPtr<Object> l2(&parent);

        l = []() { return TPtr<A>(nullptr, std::make_shared<A>()); }();
        l2 = l;
        REQUIRE(l.get() == l2.get());
    }

    SECTION("Operator == shared") {
        TPtr<Object> l2(&parent);

        l2 = std::make_shared<A>();
        REQUIRE(l2.get());
    }

    SECTION("Operator == nullptr") {
        TPtr<Object> l2(&parent);

        l2 = nullptr;
        REQUIRE(l2.expired());
    }

    SECTION("Constructor with cast") {
        TPtr<A> l(&parent);
        TPtr<Object> l2(&parent, std::make_shared<A>());

        REQUIRE_NOTHROW(l2->name);
        l2 = l;
        REQUIRE(l2.expired());
    }

    SECTION("Cast of types") {
        class B : public Object {

        };

        TPtr<A> l(&parent);
        l = std::make_shared<A>();

        TPtr<Object> base = l;
        REQUIRE(!base.expired());

        TPtr<B> lb = l;
        REQUIRE(lb.expired());

        lb = dynamic_pointer_cast<B>(base);
        REQUIRE(lb.expired());

        TPtr<B> bb(&parent, std::make_shared<A>());
        REQUIRE(bb.expired());
    }

    SECTION("move assignment") {
        TPtr<A> l(&parent);
        auto copy = std::make_shared<A>();
        l = copy;

        TPtr<A> m(&parent);
        m = std::make_shared<A>();

        l = std::move(m);

        REQUIRE(m.expired());
        REQUIRE(copy.unique());
        REQUIRE(l->onDestroySignal.slot_count() == 1);

    }
}
