#ifndef SORT_TEST_HPP_
#define SORT_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Sorting algorithms sort correctly", "[sort]")
{
    const Vector<int> expected = {1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};

    SECTION("quicksort sorts a vector")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        quicksort(values);
        CHECK(values == expected);
    }

    SECTION("quicksort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        quicksort_by(values, 0, values.size() - 1, efp::lt_v<int>);
        CHECK(values == expected);
    }

    SECTION("heapsort sorts a vector")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        heapsort(values);
        CHECK(values == expected);
    }

    SECTION("heapsort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        heapsort_by(values, efp::lt_v<int>);
        CHECK(values == expected);
    }

    SECTION("insertion_sort sorts a vector")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        insertion_sort(values);
        CHECK(values == expected);
    }

    auto greater_than = [](int a, int b)
    { return a > b; };

    // Expected result for descending order
    Vector<int> expected_descending = {9, 6, 5, 5, 5, 4, 3, 3, 2, 1, 1};

    SECTION("quicksort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        quicksort_by(values, 0, values.size() - 1, greater_than);
        CHECK(values == expected_descending);
    }

    SECTION("heapsort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        heapsort_by(values, greater_than);
        CHECK(values == expected_descending);
    }

    SECTION("insertion_sort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        insertion_sort_by(values, greater_than);
        CHECK(values == expected_descending);
    }

    SECTION("introsort_by sorts a vector with custom comparison")
    {
        Vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        introsort_by(values, greater_than);
        CHECK(values == expected_descending);
    }
}

#endif