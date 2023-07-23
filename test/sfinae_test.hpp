#ifndef SFINAE_TEST_HPP_
#define SFINAE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("all_v")
{
    CHECK(all_v() == true);
    CHECK(all_v(true) == true);
    CHECK(all_v(false) == false);
    CHECK(all_v(true, false) == false);
    CHECK(all_v(false, true) == false);
}

TEST_CASE("any_v")
{
    CHECK(any_v() == false);
    CHECK(any_v(true) == true);
    CHECK(any_v(false) == false);
    CHECK(any_v(true, false) == true);
    CHECK(any_v(false, true) == true);
}

TEST_CASE("maximum_v")
{
    CHECK(maximum_v(0) == 0);
    CHECK(maximum_v(0, 1) == 1);
    CHECK(maximum_v(-1, 1) == 1);
    CHECK(maximum_v(1., 2., 3) == 3.);
    CHECK(maximum_v(-1., 2, 3.) == 3.);
}

TEST_CASE("minimum_v")
{
    CHECK(minimum_v(0) == 0);
    CHECK(minimum_v(1, 0) == 0);
    CHECK(minimum_v(1u, -1) == 1);
    CHECK(minimum_v(1, 2., 3) == 1);
    CHECK(minimum_v(1, 2, -3.) == -3);
}

#endif