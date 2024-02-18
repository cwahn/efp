#ifndef FORMAT_TEST_HPP_
#define FORMAT_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp/format.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("format", "[format]") {
    SECTION("fundamental types") {
        CHECK(format("{}", 42) == "42");

        CHECK(format("{}", 42.0) == "42");
        CHECK(format("{}", 42.1) == "42.1");

        CHECK(format("{}", "hello") == "hello");
        CHECK(format("{}", 'c') == "c");

        CHECK(format("{}", 42u) == "42");
        CHECK(format("{}", 42l) == "42");
        CHECK(format("{}", 42ll) == "42");

        CHECK(format("{}", 42.0f) == "42");
        CHECK(format("{}", 42.1f) == "42.1");

        CHECK(format("{}", 42.0l) == "42");
        CHECK(format("{}", 42.1l) == "42.1");

        // format specifiers
        CHECK(format("{:d}", 42) == "42");
        CHECK(format("{:f}", 42.0) == "42.000000");
        CHECK(format("{:f}", 42.1) == "42.100000");
        CHECK(format("{:s}", "hello") == "hello");
        CHECK(format("{:c}", 'c') == "c");
        // CHECK(format("{:u}", 42u) == "42");
        // CHECK(format("{:l}", 42l) == "42");
        // CHECK(format("{:ll}", 42ll) == "42");
        CHECK(format("{:f}", 42.0f) == "42.000000");
        CHECK(format("{:f}", 42.1f) == "42.099998");
        CHECK(format("{:f}", 42.0l) == "42.000000");
        CHECK(format("{:f}", 42.1l) == "42.100000");
    }

    SECTION("efp::Array") {
        CHECK(format("{}", array_3) == "[1, 2, 3]");
        CHECK(format("{}", array_5) == "[1, 2, 3, 4, 5]");
    }

    SECTION("efp::ArrVec") {
        CHECK(format("{}", arrvec_3) == "[1, 2, 3]");
        CHECK(format("{}", arrvec_5) == "[1, 2, 3, 4, 5]");
    }

    SECTION("efp::Vector") {
        CHECK(format("{}", vector_3) == "[1, 2, 3]");
        CHECK(format("{}", vector_5) == "[1, 2, 3, 4, 5]");
    }

    SECTION("efp::String") {
        CHECK(format("{}", String("hello")) == "hello");
    }
}

#endif