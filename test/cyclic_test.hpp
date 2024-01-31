#ifndef CYCLIC_TEST_HPP_
#define CYCLIC_TEST_HPP_

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Vcb Rule of 5", "Vcb") {
    // use MockHW and MockRaii to check if the rule of 5 is followed
    SECTION("New Construction", "Vcb") {
        SECTION("Trivially Copiable") {
            // todo with int
        }
        SECTION("Non-Trivially Copiable") {
            {
                MockHW::reset();
                Vcb<MockRaii, 3> vcb;
                CHECK(MockHW::remaining_resource_count() == 6);
                // CHECK(MockHW::resource_state_to_string() == "1, 2, 3, 4, 5, 6");
            }
            CHECK(MockHW::is_sound());
        }
    }

    SECTION("Copy Construction", "Vcb") {
        SECTION("Trivially Copiable") {
            // todo with int
        }
        SECTION("Non-Trivially Copiable") {
            {
                MockHW::reset();
                Vcb<MockRaii, 3> vcb;
                CHECK(MockHW::remaining_resource_count() == 6);
                CHECK(MockHW::resource_state_to_int() == 123456);

                Vcb<MockRaii, 3> vcb_copy = vcb;
                CHECK(MockHW::remaining_resource_count() == 12);
                // CHECK(MockHW::resource_state_to_string() == "1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12");
            }
            CHECK(MockHW::is_sound());
        }
    }

    SECTION("Copy Assignment", "Vcb") {
        SECTION("Trivially Copiable") {
            // todo with int
        }
        SECTION("Non-Trivially Copiable") {
            {
                MockHW::reset();
                Vcb<MockRaii, 3> vcb;
                CHECK(MockHW::remaining_resource_count() == 6);

                Vcb<MockRaii, 3> vcb_copy;
                CHECK(MockHW::remaining_resource_count() == 12);

                vcb_copy = vcb;
                CHECK(MockHW::remaining_resource_count() == 12);
            }
            CHECK(MockHW::is_sound());
        }
    }

    SECTION("Move Construction", "Vcb") {
        SECTION("Trivially Copiable") {
            // todo with int
        }
        SECTION("Non-Trivially Copiable") {
            {
                MockHW::reset();
                Vcb<MockRaii, 3> vcb;
                CHECK(MockHW::remaining_resource_count() == 6);

                Vcb<MockRaii, 3> vcb_move = std::move(vcb);
                CHECK(MockHW::remaining_resource_count() == 6);
            }
            CHECK(MockHW::is_sound());
        }
    }

    SECTION("Move Assignment", "Vcb") {
        SECTION("Trivially Copiable") {
            // todo with int
        }
        SECTION("Non-Trivially Copiable") {
            {
                MockHW::reset();
                Vcb<MockRaii, 3> vcb;
                CHECK(MockHW::remaining_resource_count() == 6);

                Vcb<MockRaii, 3> vcb_move;
                CHECK(MockHW::remaining_resource_count() == 12);

                vcb_move = std::move(vcb);
                CHECK(MockHW::remaining_resource_count() == 6);
            }
            CHECK(MockHW::is_sound());
        }
    }
}

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
        // NonTrivial::reset();
        // CHECK(NonTrivial::construction_cnt == 0);
        // CHECK(NonTrivial::destruction_cnt == 0);

        // Vcq<NonTrivial, 3> vcq;
        // CHECK(NonTrivial::construction_cnt == 6);
        // CHECK(NonTrivial::destruction_cnt == 0);

        // vcq.push_back(NonTrivial{});
        // CHECK(NonTrivial::construction_cnt == 9);
        // CHECK(NonTrivial::destruction_cnt == 3);

        // vcq.push_back(NonTrivial{});
        // CHECK(NonTrivial::construction_cnt == 12);
        // CHECK(NonTrivial::destruction_cnt == 6);

        // const auto a = vcq.pop_front();
        // CHECK(NonTrivial::construction_cnt == 13);
        // CHECK(NonTrivial::destruction_cnt == 8);
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