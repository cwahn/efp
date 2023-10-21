#ifndef SM_TEST_HPP_
#define SM_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Pure")
{
    SECTION("0")
    {
        auto id_sm = pure_sm([](int x)
                             { return x; });

        CHECK(id_sm(0) == 0);
        CHECK(id_sm(42) == 42);
    }
}

TEST_CASE("Sm compose")
{
    SECTION("left-identity")
    {
        // auto id_sm = pure_sm(id<int>);
        auto id_sm = pure_sm([](int x)
                             { return x; });
        auto times_2_sm = pure_sm([](int x)
                                  { return 2 * x; });

        // CHECK(IsSm<decltype(id_sm)>::value == true);
        // CHECK(IsSm<Pure<decltype(id<int>)>>::value == true);
        // CHECK(IsSm<decltype(times_2)>::value == true);

        auto composed = sm_compose(id_sm, times_2_sm);
        CHECK(composed(21) == 42);
    }
}

#endif