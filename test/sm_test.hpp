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
        auto id_sm = pure_sm(id<int>);

        CHECK(id_sm(0) == 0);
        CHECK(id_sm(42) == 42);
    }
}

#endif