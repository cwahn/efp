#ifndef CYCLIC_TEST_HPP_
#define CYCLIC_TEST_HPP_

#include "efp.hpp"

using namespace efp;

// type having checkable side effect on construction and destruction
struct SideEffectType {
    static int construction_count;
    static int destruction_count;
    static void reset() {
        construction_count = 0;
        destruction_count = 0;
    }
    SideEffectType() {
        ++construction_count;
    }
    SideEffectType(const SideEffectType&) {
        ++construction_count;
    }
    SideEffectType(SideEffectType&&) {
        ++construction_count;
    }
    ~SideEffectType() {
        ++destruction_count;
    }
};

int SideEffectType::construction_count = 0;
int SideEffectType::destruction_count = 0;

TEST_CASE("Vcb") {
    SECTION("0") {
        Vcb<int, 3> vcb;
        CHECK(IsSame<Element<decltype(vcb)>, int>::value == true);
        CHECK(vcb.size() == 3);
        CHECK(length(vcb) == 3);
        CHECK(IsIntegralConst<decltype(length(vcb))>::value == true);
        CHECK(vcb.empty() == false);

        vcb.push_back(1);
        vcb.push_back(2);
        vcb.push_back(3);
        vcb.push_back(4);

        CHECK(vcb[0] == 2);
        CHECK(vcb[1] == 3);
        CHECK(vcb[2] == 4);
    }

    // Through test of it works with non-trivially copyable type by using custom type having side effect on construction and destruction and destruction.
    SECTION("Non-Trivially Copiable") {

        SideEffectType::reset();
        CHECK(SideEffectType::construction_count == 0);
        CHECK(SideEffectType::destruction_count == 0);

        Vcb<SideEffectType, 3> vcb;
        CHECK(SideEffectType::construction_count == 6);
        CHECK(SideEffectType::destruction_count == 0);

        vcb.push_back(SideEffectType{});
        CHECK(SideEffectType::construction_count == 9);
        CHECK(SideEffectType::destruction_count == 3);

        vcb.push_back(SideEffectType{});
        CHECK(SideEffectType::construction_count == 12);
        CHECK(SideEffectType::destruction_count == 6);
    }
}

TEST_CASE("Vcq") {
    SECTION("Array") {
        Vcq<int, 3> vcq;
        CHECK(IsSame<Element<decltype(vcq)>, int>::value == true);
        CHECK(vcq.size() == 0);
        CHECK(vcq.empty() == true);
        CHECK(length(vcq) == 0);
        CHECK(IsIntegralConst<decltype(length(vcq))>::value == false);

        vcq.push_back(1);

        CHECK(vcq.size() == 1);
        CHECK(vcq.empty() == false);
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

    SECTION("Non-Trivially Copiable") {
        SideEffectType::reset();
        CHECK(SideEffectType::construction_count == 0);
        CHECK(SideEffectType::destruction_count == 0);

        Vcq<SideEffectType, 3> vcq;
        CHECK(SideEffectType::construction_count == 6);
        CHECK(SideEffectType::destruction_count == 0);

        vcq.push_back(SideEffectType{});
        CHECK(SideEffectType::construction_count == 9);
        CHECK(SideEffectType::destruction_count == 3);

        vcq.push_back(SideEffectType{});
        CHECK(SideEffectType::construction_count == 12);
        CHECK(SideEffectType::destruction_count == 6);

        const auto a = vcq.pop_front();
        CHECK(SideEffectType::construction_count == 13);
        CHECK(SideEffectType::destruction_count == 8);
    }
}

// TEST_CASE("BufferArrVec")
// {
//     SECTION("0")
//     {
//         BufferArrVec<int, 3> as;
//         CHECK(IsSame<Element<decltype(as)>, int>::value == true);
//         CHECK(length(as) == 0);
//         CHECK(length(as) == 0);
//         CHECK(IsIntegralConst<decltype(length(as))>::value == false);
//         CHECK(as.empty() == false);

//         as.push_back(1);
//         CHECK(as[2] == 1);
//         CHECK(length(as) == 1);

//         as.push_back(2);
//         CHECK(as[1] == 1);
//         CHECK(as[2] == 2);
//         as.push_back(3);
//         as.push_back(4);

//         CHECK(as[0] == 2);
//         CHECK(as[1] == 3);
//         CHECK(as[2] == 4);
//         CHECK(length(as) == 3);
//     }
// }

#endif