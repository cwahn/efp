#ifndef SORT_TEST_HPP_
#define SORT_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Sorting algorithms sort correctly", "[sort]")
{
    Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    Vector<int> expectedAscending = {1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};
    Vector<int> expectedDescending = {9, 6, 5, 5, 5, 4, 3, 3, 2, 1, 1};
    bool (*less_than)(const int &, const int &) = efp::lt_v<int>;
    auto greater_than = [](int a, int b)
    { return a > b; };

    SECTION("quicksort sorts a vector in ascending order")
    {
        Vector<int> testValues = values;
        quicksort(testValues);
        CHECK(testValues == expectedAscending);
    }

    SECTION("heapsort sorts a vector in ascending order")
    {
        Vector<int> testValues = values;
        heapsort(testValues);
        CHECK(testValues == expectedAscending);
    }

    SECTION("insertion_sort sorts a vector in ascending order")
    {
        Vector<int> testValues = values;
        insertion_sort(testValues);
        CHECK(testValues == expectedAscending);
    }

    SECTION("timsort sorts a vector in ascending order")
    {
        Vector<int> testValues = values;
        timsort(testValues);
        CHECK(testValues == expectedAscending);
    }

    SECTION("quicksort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> testValues = values;
        quicksort_by(testValues, 0, testValues.size() - 1, greater_than);
        CHECK(testValues == expectedDescending);
    }

    SECTION("heapsort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> testValues = values;
        heapsort_by(testValues, greater_than);
        CHECK(testValues == expectedDescending);
    }

    SECTION("insertion_sort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> testValues = values;
        insertion_sort_by(testValues, greater_than);
        CHECK(testValues == expectedDescending);
    }

    SECTION("introsort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> testValues = values;
        introsort_by(testValues, greater_than);
        CHECK(testValues == expectedDescending);
    }
}

#endif