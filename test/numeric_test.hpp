#ifndef NUMERIC_TEST_HPP_
#define NUMERIC_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("real_from_complex") {
    SECTION("real") {
        const Complex<int> a{1, 0};
        const Complex<double> b{1., 0.};

        CHECK(real_from_complex(a).value() == 1);
        CHECK(is_approx(real_from_complex(b).value(), 1.));
    }

    SECTION("not real") {
        const Complex<int> a{1, 1};
        const Complex<double> b{1., 1.};

        CHECK(real_from_complex(a).is_nothing());
        CHECK(real_from_complex(b).is_nothing());
    }
}

TEST_CASE("complex_cast") {
    const int real_a{42};
    const Complex<int> complex_a{42, 0};

    CHECK(complex_cast<true>(real_a) == complex_a);
    CHECK(complex_cast<true>(complex_a) == complex_a);
    CHECK(complex_cast<false>(real_a) == real_a);
    CHECK(complex_cast<false>(complex_a) == real_a);
}

TEST_CASE("max_elem") {
    SECTION("c style ") {
        CHECK(max_elem(array_5) == 5);
    }

    SECTION("std::array ") {
        CHECK(max_elem(array_5) == 5.);
    }

    SECTION("std::vectors") {
        CHECK(max_elem(vector_5) == 5.);
    }
}

TEST_CASE("min_elem") {
    SECTION("c style ") {
        CHECK(min_elem(array_5) == 1);
    }

    SECTION("std::array ") {
        CHECK(min_elem(array_5) == 1.);
    }

    SECTION("std::vectors") {
        CHECK(min_elem(vector_5) == 1.);
    }
}

TEST_CASE("sum") {
    SECTION("c style ") {
        CHECK(sum(array_5) == 15);
    }

    SECTION("std::array ") {
        CHECK(sum(array_5) == 15.);
    }

    SECTION("std::vectors") {
        CHECK(sum(vector_5) == 15.);
    }
}

TEST_CASE("product") {
    SECTION("c style ") {
        CHECK(product(array_5) == 120);
    }

    SECTION("std::array ") {
        CHECK(product(array_5) == 120.);
    }

    SECTION("std::vectors") {
        CHECK(product(vector_5) == 120.);
    }
}

#endif