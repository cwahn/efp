#ifndef MAYBE_TEST_HPP_
#define MAYBE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("maybe_construction")
{
    SECTION("Nothing")
    {
        Maybe<int> a = Nothing{};

        CHECK(a.has_value() == false);
        CHECK(!a);
    }

    SECTION("Just")
    {
        Maybe<int> a = 42;

        CHECK(a.has_value() == true);
        CHECK(a.value() == 42);
        CHECK(a);
    }
}

TEST_CASE("maybe_functor")
{
    Maybe<int> a = 2;
    const auto square = [](int x)
    { return x * x; };

    CHECK(fmap(square, a).index() == 1);
    CHECK(fmap(square, a).is_nothing() == false);
    CHECK(fmap(square, a).value() == 4);
}

int times_2(int x)
{
    return 2 * x;
}

TEST_CASE("maybe_applicative")
{

    SECTION("pure")
    {
        CHECK(pure<Maybe<int>>(2).value() == 2);
    }

    // ! Can't copy assign lambda

    // auto times_2 = [](int x)
    // {
    //     return 2 * x;
    // };

    // ! Can't do sizeof to function pointer

    // Maybe<decltype(times_2)> maybe_times_2 = &times_2;

    // SECTION("ap")
    // {
    //     Maybe<int> ma = 2;
    //     CHECK(ap(maybe_times_2, ma).value() == 4);
    // }
}

TEST_CASE("maybe_monad")
{
    SECTION("bind")
    {
        Maybe<int> ma = 2;

        const auto div_12_by = [](int x)
            -> Maybe<double>
        {
            if (x == 0)
            {
                return Nothing{};
            }

            return Maybe<double>{12. / x};
        };

        CHECK(bind(ma, div_12_by).is_nothing() == false);
        CHECK(bind(ma, div_12_by).value() == 6.);

        Maybe<int> mb = 0;

        CHECK(bind(mb, div_12_by).is_nothing() == true);

        Maybe<int> mc = Nothing{};

        CHECK(bind(mc, div_12_by).is_nothing() == true);
    }

    SECTION(">>=")
    {
        Maybe<int> ma = 2;

        const auto div_12_by = [](int x)
            -> Maybe<double>
        {
            if (x == 0)
            {
                return Nothing{};
            }

            return Maybe<double>{12. / x};
        };

        CHECK((ma >>= div_12_by).is_nothing() == false);
        CHECK(bind(ma, div_12_by).value() == 6.);

        Maybe<int> mb = 0;

        CHECK((mb >>= div_12_by).is_nothing() == true);

        Maybe<int> mc = Nothing{};

        CHECK((mc >>= div_12_by).is_nothing() == true);
    }
}

#endif