#ifndef CRTP_PRELUDE_TEST_HPP_
#define CRTP_PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "crtp_prelude.hpp"

using namespace efp;

TEST_CASE("for_each")
{
    Arr<int, 3> crtp_array{1, 2, 3};

    int res_1 = 0;
    auto add_to_res_1 = [&](int x)
    { res_1 += x; };

    for_each(add_to_res_1, crtp_array);
    CHECK(res_1 == 6);
}

#endif