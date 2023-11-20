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
    Vector<int> expected_descending = {9, 6, 5, 5, 5, 4, 3, 3, 2, 1, 1};

    bool (*less_than)(const int &, const int &) = efp::lt_v<int>;

    auto greater_than = [](int a, int b)
    { return a > b; };

    SECTION("quicksort sorts a vector in ascending order")
    {
        Vector<int> test_values = values;
        quicksort(test_values);
        CHECK(test_values == expectedAscending);
    }

    SECTION("heapsort sorts a vector in ascending order")
    {
        Vector<int> test_values = values;
        heapsort(test_values);
        CHECK(test_values == expectedAscending);
    }

    SECTION("insertion_sort sorts a vector in ascending order")
    {
        Vector<int> test_values = values;
        insertion_sort(test_values);
        CHECK(test_values == expectedAscending);
    }

    SECTION("timsort sorts a vector in ascending order")
    {
        Vector<int> test_values = values;
        timsort(test_values);
        CHECK(test_values == expectedAscending);
    }

    SECTION("quicksort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> test_values = values;
        quicksort_by(test_values, 0, test_values.size() - 1, greater_than);
        CHECK(test_values == expected_descending);
    }

    SECTION("heapsort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> test_values = values;
        heapsort_by(test_values, greater_than);
        CHECK(test_values == expected_descending);
    }

    SECTION("insertion_sort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> test_values = values;
        insertion_sort_by(test_values, greater_than);
        CHECK(test_values == expected_descending);
    }

    SECTION("size_trosort_by sorts a vector in descending order with custom comparison")
    {
        Vector<int> test_values = values;
        size_trosort_by(test_values, greater_than);
        CHECK(test_values == expected_descending);
    }
}

#endif