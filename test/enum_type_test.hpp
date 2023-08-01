#ifndef ENUM_TYPE_TEST_HPP_
#define ENUM_TYPE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

struct None
{
};

using IntOrDouble = Enum<int, double>;

TEST_CASE("enum_type")
{
    CHECK(sizeof(IntOrDouble) == 16);    
    IntOrDouble a = 1;
    CHECK(a.index() == 0);
    CHECK(a.get<int>() == 1);
    CHECK(a.get<0>() == 1);

    IntOrDouble b = 2.;
    CHECK(b.index() == 1);
    CHECK(b.get<double>() == 2.);
    CHECK(b.get<1>() == 2.);
    // ! Not catched because of abort()
    // CHECK(b.get<int>() == 1);
}

#endif