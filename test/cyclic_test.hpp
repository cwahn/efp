#ifndef CYCLIC_TEST_HPP_
#define CYCLIC_TEST_HPP_

#include "efp.hpp"

using namespace efp;

TEST_CASE("Vcb")
{
    SECTION("0")
    {
        Vcb<int, 3> vcb;
        CHECK(IsSame<Element_t<decltype(vcb)>, int>::value == true);
        CHECK(vcb.size() == 3);
        CHECK(length(vcb) == 3);
        CHECK(IsIntegralConst<decltype(length(vcb))>::value == false);
        CHECK(vcb.is_empty() == false);

        vcb.push_back(1);
        vcb.push_back(2);
        vcb.push_back(3);
        vcb.push_back(4);

        CHECK(vcb[0] == 2);
        CHECK(vcb[1] == 3);
        CHECK(vcb[2] == 4);
    }
}

TEST_CASE("Vcq")
{
    SECTION("Array")
    {
        Vcq<int, 3> vcq;
        CHECK(IsSame<Element_t<decltype(vcq)>, int>::value == true);
        CHECK(vcq.size() == 0);
        CHECK(vcq.is_empty() == true);
        CHECK(length(vcq) == 0);
        CHECK(IsIntegralConst<decltype(length(vcq))>::value == false);

        vcq.push_back(1);

        CHECK(vcq.size() == 1);
        CHECK(vcq.is_empty() == false);
        CHECK(vcq.pop_front() == 1);

        vcq.push_back(2);
        vcq.push_back(3);
        vcq.push_back(4);

        CHECK(vcq.size() == 3);
        CHECK(vcq.pop_front() == 2);
        CHECK(vcq.pop_front() == 3);
        CHECK(vcq.pop_front() == 4);

        vcq.push_back(5);
        vcq.push_back(6);
        vcq.push_back(7);
        vcq.push_back(8);

        CHECK(vcq.size() == 3);
        CHECK(vcq[0] == 6);
        CHECK(vcq[1] == 7);
        CHECK(vcq[2] == 8);
        CHECK(vcq.pop_front() == 6);
        CHECK(vcq.pop_front() == 7);
        CHECK(vcq.pop_front() == 8);
    }
}

TEST_CASE("Csb")
{
    SECTION("0")
    {
        Csb<int, 3> csb;
        CHECK(IsSame<Element_t<decltype(csb)>, int>::value == true);
        CHECK(csb.size() == 3);
        CHECK(length(csb) == 3);
        CHECK(IsIntegralConst<decltype(length(csb))>::value == false);
        CHECK(csb.is_empty() == false);

        csb.push_back(1);
        CHECK(csb[0] == 1);
        csb.push_back(2);
        CHECK(csb[0] == 1);
        CHECK(csb[1] == 2);
        csb.push_back(3);
        csb.push_back(4);

        CHECK(csb[0] == 2);
        CHECK(csb[1] == 3);
        CHECK(csb[2] == 4);
    }
}

#endif