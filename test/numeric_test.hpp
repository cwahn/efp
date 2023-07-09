#ifndef NUMERIC_TEST_HPP_
#define NUMERIC_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "test_common.hpp"
#include "numeric.hpp"

TEST_CASE("maximum")
{
    SECTION("c style ")
    {
        CHECK(maximum(c_array_5) == 5);
    }

    SECTION("std::array ")
    {
        CHECK(maximum(std_array_5) == 5.);
    }

    SECTION("std::vectors")
    {
        CHECK(maximum(std_vector_5) == 5.);
    }
}

TEST_CASE("minimum")
{
    SECTION("c style ")
    {
        CHECK(minimum(c_array_5) == 1);
    }

    SECTION("std::array ")
    {
        CHECK(minimum(std_array_5) == 1.);
    }

    SECTION("std::vectors")
    {
        CHECK(minimum(std_vector_5) == 1.);
    }
}

TEST_CASE("sum")
{
    SECTION("c style ")
    {
        CHECK(sum(c_array_5) == 15);
    }

    SECTION("std::array ")
    {
        CHECK(sum(std_array_5) == 15.);
    }

    SECTION("std::vectors")
    {
        CHECK(sum(std_vector_5) == 15.);
    }
}

TEST_CASE("product")
{
    SECTION("c style ")
    {
        CHECK(product(c_array_5) == 120);
    }

    SECTION("std::array ")
    {
        CHECK(product(std_array_5) == 120.);
    }

    SECTION("std::vectors")
    {
        CHECK(product(std_vector_5) == 120.);
    }
}

TEST_CASE("mean")
{
    SECTION("c style ")
    {
        CHECK(mean(c_array_5) == 3.);
    }

    SECTION("std::array ")
    {
        CHECK(mean(std_array_5) == 3.);
    }

    SECTION("std::vectors")
    {
        CHECK(mean(std_vector_5) == 3.);
    }
}

TEST_CASE("arange")
{
    SECTION("int")
    {
        std::vector<int> ref = {0, 1, 2};
        CHECK(arange(0, 3, 1) == ref);
    }

    SECTION("double")
    {
        std::vector<double> ref = {0., 1., 2.};
        CHECK(arange(0., 3., 1.) == ref);
    }
}

#endif