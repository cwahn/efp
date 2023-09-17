#ifndef NUMERIC_TEST_HPP_
#define NUMERIC_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("real_from_complex")
{
    SECTION("real")
    {
        const Complex<int> a{1, 0};
        const Complex<double> b{1., 0.};

        CHECK(real_from_complex(a).value() == 1);
        CHECK(is_approx(real_from_complex(b).value(), 1.));
    }

    SECTION("not real")
    {
        const Complex<int> a{1, 1};
        const Complex<double> b{1., 1.};

        CHECK(real_from_complex(a).is_nothing());
        CHECK(real_from_complex(b).is_nothing());
    }
}

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

#endif