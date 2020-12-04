//
// Created by Michal_Marszalek on 10.09.2020.
//
#define TEST

#include "catch.hpp"

#undef WARN
#undef INFO

#include <GameClient/TPtr.h>
#include <GameClient/Unity/Core/Object.h>

namespace std {
    template<class T>
    shared_ptr(T *) ->  shared_ptr<T>;
}

TEST_CASE("TPtr") {
    class A : public Object {
    public:
        ~A() {
            if (d)d();
        }

        void works() {}

        std::function<void()> d;
    };

    SECTION("CREATE") {
        TPtr<A> l(std::make_shared<A>());

        REQUIRE(l);
    }

    SECTION("Test deleting ptr and throw") {
        TPtr<A> l;

        l = std::make_shared<A>();

        REQUIRE_NOTHROW(l->works());

        l->onDestroySignal(nullptr);

        REQUIRE_THROWS(l->works());
        REQUIRE(l.expired());
    }

    SECTION("TPtr destructor") {
        auto p = std::make_shared<A>();
        {
            TPtr<A> l;

            l = [=]() { return p; }();

            REQUIRE(p->onDestroySignal.slot_count() == 1);
        }

        REQUIRE(p->onDestroySignal.slot_count() == 0);
    }

    SECTION("Clear pointer") {
        TPtr<A> l;

        l = std::make_shared<A>();
        REQUIRE_NOTHROW(l->works());

        l = nullptr;

        REQUIRE_THROWS(l->works());
    }

    SECTION("Check define and destructor") {
        struct B : Object {
            TPtr<> a;
            TPtr<A> b;
        };
        TPtr<B> m;
        {
            TPtr<B> b = std::make_shared<B>();
            m = b;

            b->a = std::make_shared<A>();
            b->b = std::make_shared<A>();

            int i = 0;
            dynamic_cast<A *>(b->a.get())->d = [&i]() { i += 1; };
            b->b->d = [&i]() { i += 1; };

            b->onDestroySignal(nullptr);

            REQUIRE(i == 2);
        }
        REQUIRE(m.expired());
    }

    SECTION("Operator == ") {
        TPtr<A> l;
        TPtr<A> l2;

        l = std::make_shared<A>();
        l2 = l;
        REQUIRE(l.get() == l2.get());
    }

    SECTION("Operator == Base") {
        TPtr<A> l;
        TPtr<Object> l2;

        l = std::make_shared<A>();
        l2 = l;
        REQUIRE(l.get() == l2.get());
    }

    SECTION("Operator = shared") {
        TPtr<Object> l2;

        l2 = std::make_shared<A>();
        REQUIRE(l2.get());
    }

    SECTION("Operator = nullptr") {
        TPtr<Object> l2;

        l2 = std::make_shared<A>();
        l2 = nullptr;
        REQUIRE(l2.expired());
    }

    SECTION("Operator = move") {
        TPtr<Object> l2(std::shared_ptr(new A));
        TPtr<A> l(std::shared_ptr(new A));

        REQUIRE_NOTHROW(l2->name);
        l = dynamic_pointer_cast<A>(std::move(l2));
        REQUIRE(l2.expired());
        REQUIRE(l->onDestroySignal.slot_count() == 1);

        l2 = static_pointer_cast<Object>(std::move(l));
        REQUIRE(l.expired());
        REQUIRE(l2->onDestroySignal.slot_count() == 1);

        l = dynamic_pointer_cast<A>(std::move(l2));
        l2 = std::move(l);
        REQUIRE(l.expired());
        REQUIRE(l2->onDestroySignal.slot_count() == 1);
    }

    SECTION("Constructor with cast") {
        TPtr<A> l;
        TPtr<Object> l2(std::make_shared<A>());

        REQUIRE_NOTHROW(l2->name);
        l2 = l;
        REQUIRE(l2.expired());
    }

    SECTION("Constructor with new") {
        TPtr<A> l;
        TPtr<Object> l2(std::shared_ptr(new A));

        REQUIRE_NOTHROW(l2->name);
        l2 = l;
        REQUIRE(l2.expired());
    }

    SECTION("Constructor with move") {
        TPtr<Object> l2(std::shared_ptr(new A));

        REQUIRE_NOTHROW(l2->name);
        TPtr<A> l(std::move(l2));
        REQUIRE(l2.expired());
        REQUIRE(l->onDestroySignal.slot_count() == 1);
    }

    SECTION("Cast of types") {
        class B : public Object {

        };

        TPtr<A> l;
        l = std::make_shared<A>();

        TPtr<Object> base = l;
        REQUIRE(!base.expired());

        TPtr<B> lb = dynamic_pointer_cast<B>(l);
        REQUIRE(lb.expired());

        lb = dynamic_pointer_cast<B>(base);
        REQUIRE(lb.expired());

        TPtr<B> bb(std::make_shared<A>());
        REQUIRE(bb.expired());
    }

    SECTION("move assignment") {
        TPtr<A> l;
        auto copy = std::make_shared<A>();
        l = copy;

        TPtr<A> m;
        m = std::make_shared<A>();

        l = std::move(m);

        REQUIRE(m.expired());
        REQUIRE(copy.use_count() == 1);
        REQUIRE(l->onDestroySignal.slot_count() == 1);

    }

    SECTION("shared_from_this") {
        TPtr<A> l;
        l = std::make_shared<A>();
        TPtr<A> c;
        REQUIRE_NOTHROW((c = dynamic_pointer_cast<A>(l->shared_from_this())));
        REQUIRE(c.get() == l.get());
        REQUIRE(c.use_count() == 2);
    }
}
