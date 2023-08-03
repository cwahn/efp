#ifndef ENUM_TYPE_TEST_HPP_
#define ENUM_TYPE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

struct None
{
};

TEST_CASE("enum_type")
{
    CHECK(sizeof(Enum<int, double>) == 16);
    Enum<int, double> a = 1;
    CHECK(a.index() == 0);
    CHECK(a.get<int>() == 1);
    CHECK(a.get<0>() == 1);

    Enum<int, double> b = 2.;
    CHECK(b.index() == 1);
    CHECK(b.get<double>() == 2.);
    CHECK(b.get<1>() == 2.);
    // ! Not catched because of abort()
    // CHECK(b.get<int>() == 1);
}

TEST_CASE("enum_match")
{
    SECTION("void0")
    {
        Enum<int, double> a = 42;
        double b = 0.;

        a.match(
            [&](int x)
            { b += x; },
            [&](double x)
            { b++; });

        //! Non-exhaustive match will not compile

        // a.match(
        //     [&](Unit x)
        //     { b = -1.; });

        CHECK(b == 42.);
    }

    SECTION("void1")
    {
        Enum<int, double> a = 42.;
        double b = 0.;

        a.match(
            [&](int x)
            { b += x; },
            [&](double x)
            { b++; });

        CHECK(b == 1.);
    }

    SECTION("non-void0")
    {
        Enum<Unit, int> a = Unit{};

        int b = a.match(
            [&](Unit x)
            { return -1; },
            [&](int x)
            { return 1; });

        CHECK(b == -1);
    }

    SECTION("non-void1")
    {
        Enum<Unit, int> a = 42;

        int b = a.match(
            [&](Unit x)
            { return -1; },
            [&](int x)
            { return x; });

        CHECK(b == 42);
    }

    SECTION("non-void2")
    {
        Enum<bool, int, double> a = 0;
        int b = 42;

        int c = a.match(
            [&](bool x)
            { return b * 0; },
            [&](int x)
            { return b * 1; },
            [&](double x)
            { return b * 2; });

        CHECK(b == 42);
    }
}

#endif