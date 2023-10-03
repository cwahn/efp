#ifndef TUPLE_TEST_HPP_
#define TUPLE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Tuple")
{
    SECTION("const")
    {
        const Tuple<bool, int> tpl{true, 42};
        CHECK(tpl.template get<0>() == true);
        CHECK(tpl.template get<1>() == 42);
        CHECK(get<0>(tpl) == true);
        CHECK(get<1>(tpl) == 42);
    }

    SECTION("non const")
    {
        Tuple<bool, int> tpl{true, 42};
        CHECK(tpl.template get<0>() == true);
        CHECK(tpl.template get<1>() == 42);
        CHECK(get<0>(tpl) == true);
        CHECK(get<1>(tpl) == 42);

        tpl.template get<0>() = false;
        tpl.template get<1>() = 0;

        CHECK(tpl.template get<0>() == false);
        CHECK(tpl.template get<1>() == 0);
        CHECK(get<0>(tpl) == false);
        CHECK(get<1>(tpl) == 0);
    }
}

#endif