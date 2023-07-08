#ifndef NUMERIC_TEST_HPP_
#define NUMERIC_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "test_common.hpp"
#include "numeric.hpp"

TEST_CASE("maximum")
{
    SECTION("c style ")
    {
        REQUIRE(maximum(c_array_5) == 5);
    }

    SECTION("std::array ")
    {
        REQUIRE(maximum(std_array_5) == 5.);
    }

    SECTION("std::vectors")
    {
        REQUIRE(maximum(std_vector_5) == 5.);
    }
}

TEST_CASE("minimum")
{
    SECTION("c style ")
    {
        REQUIRE(minimum(c_array_5) == 1);
    }

    SECTION("std::array ")
    {
        REQUIRE(minimum(std_array_5) == 1.);
    }

    SECTION("std::vectors")
    {
        REQUIRE(minimum(std_vector_5) == 1.);
    }
}

TEST_CASE("sum")
{
    SECTION("c style ")
    {
        REQUIRE(sum(c_array_5) == 15);
    }

    SECTION("std::array ")
    {
        REQUIRE(sum(std_array_5) == 15.);
    }

    SECTION("std::vectors")
    {
        REQUIRE(sum(std_vector_5) == 15.);
    }
}

TEST_CASE("product")
{
    SECTION("c style ")
    {
        REQUIRE(product(c_array_5) == 120);
    }

    SECTION("std::array ")
    {
        REQUIRE(product(std_array_5) == 120.);
    }

    SECTION("std::vectors")
    {
        REQUIRE(product(std_vector_5) == 120.);
    }
}

TEST_CASE("mean")
{
    SECTION("c style ")
    {
        REQUIRE(mean(c_array_5) == 3.);
    }

    SECTION("std::array ")
    {
        REQUIRE(mean(std_array_5) == 3.);
    }

    SECTION("std::vectors")
    {
        REQUIRE(mean(std_vector_5) == 3.);
    }
}

#endif