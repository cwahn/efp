#ifndef PRELUDE_TEST_HPP_
#define PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "prelude.hpp"

using namespace efp;

TEST_CASE("compose")
{
    SECTION("pure0")
    {
        auto add_1 = [](int x)
        { return x + 1; };

        auto times_2 = [](int x)
        { return x * 2; };

        auto composed = compose(add_1, times_2);
        // auto composed_with_operator = add_1 * times_2;

        CHECK(composed(2) == 5);
        // CHECK(composed_with_operator(2) == 5);
    }

    SECTION("pure1")
    {
        auto add_1 = [](int x)
        { return x + 1; };

        auto devide_2 = [](int x)
        { return x / 2.; };

        auto composed = compose(devide_2, add_1);
        CHECK(composed(2) == 1.5);
    }

    SECTION("side-effect")
    {
        int i = 0;

        auto i_plusplus = [&](int x)
        {
            i += x;
            return i; };

        auto times_2 = [](int x)
        { return x * 2; };

        auto composed = compose(times_2, i_plusplus);
        CHECK(composed(2) == 4);
        CHECK(i == 2);
    }

    SECTION("n-ary")
    {
        auto add_1 = [](int x)
        { return x + 1; };

        auto devide_2 = [](int x)
        { return x / 2.; };

        auto square = [](int x)
        { return x * x; };

        const auto composed = compose(devide_2, add_1, square);
        CHECK(composed(2) == 2.5);
    }
}

TEST_CASE("append")
{
    SECTION("Array")
    {
        Array<double, 6> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(array_3, array_3) == res);
    }

    SECTION("ArrVec")
    {
        ArrVec<double, 6> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(arrvec_3, array_3) == res);
    }

    SECTION("Array")
    {
        Vector<double> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(array_3, vector_3) == res);
    }
}

TEST_CASE("for_each")
{
    Array<int, 3> crtp_array{1, 2, 3};

    int res_1 = 0;
    auto add_to_res_1 = [&](int x)
    { res_1 += x; };

    for_each(add_to_res_1, crtp_array);
    CHECK(res_1 == 6);
}

TEST_CASE("map")
{
    auto times_2 = [](int x)
    { return 2 * x; };

    SECTION("Array")
    {
        CHECK(map(times_2, array_3) == Array<int, 3>{2, 4, 6});
    }
    SECTION("C array")
    {
        // ArrayView<double, 3> view{stl_array_3};
        // CHECK(map(times_2, StlArrayAdapter<double, 3>{stl_array_3}) == Array<int, 3>{2, 4, 6});
        // CHECK(map(times_2, stl_array_3) == Array<int, 3>{2, 4, 6});
    }
}

TEST_CASE("filter")
{
    Array<int, 3> crtp_array{1, 2, 3};

    auto is_even = [](int x)
    {
        return x % 2 == 0 ? true : false;
    };

    ArrVec<int, 3> ref{};
    ref.resize(1);
    ref[0] = 2;

    CHECK(filter(is_even, crtp_array) == ref);
}

TEST_CASE("foldl")
{
    ArrVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldl(plus, 0, crtp_arrayvec) == 10);
}

TEST_CASE("foldr")
{
    ArrVec<int, 5> crtp_arrayvec{1, 2, 3, 4};
    auto plus = [](int lhs, int rhs)
    { return lhs + rhs; };

    CHECK(foldr(plus, 0, crtp_arrayvec) == 10);
}

TEST_CASE("from_function")
{
    auto plus_one = [](int i)
    {
        return i + 1;
    };

    SECTION("Array")
    {
        CHECK(from_function(IntegralConst<int, 3>{}, plus_one) == Array<int, 3>{1, 2, 3});
    }

    SECTION("Vector")
    {
        CHECK(from_function(3, plus_one) == Vector<int>{1, 2, 3});
    }

    SECTION("template")
    {
        CHECK(from_function(IntegralConst<int, 3>{}, id<int>) == Array<int, 3>{0, 1, 2});
    }
}

TEST_CASE("for_index")
{
    int res = 0;

    auto add_to_res = [&](int i)
    {
        res += i;
    };

    for_index(add_to_res, 0);
    CHECK(res == 0);
    for_index(add_to_res, 3);
    CHECK(res == 3);
    for_index(add_to_res, -1);
    CHECK(res == 3);
}

TEST_CASE("for_each_with_index")
{
    Array<int, 3> res;

    auto product = [&](int i, int x0, int x1)
    {
        res[i] = x0 * x1;
    };

    for_each_with_index(product, array_3, vector_5);
    CHECK(res == Array<int, 3>{1, 4, 9});
}

TEST_CASE("cartesian_for_each")
{
    Vector<int> res{};
    auto as = Array<int, 2>{1, 2};
    auto bs = Array<int, 2>{1, 3};

    auto push_res = [&](int x0, int x1)
    {
        res.push_back(10 * x0 + x1);
    };

    cartesian_for_each(push_res, as, bs);
    CHECK(res == Vector<int>{11, 13, 21, 23});
}

TEST_CASE("map_with_index")
{
    SECTION("array")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, array_3) == Array<int, 3>{0, 2, 6});
    }

    SECTION("vector")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, vector_3) == Vector<double>{0., 2., 6.});
    }

    SECTION("binary hetero")
    {
        auto product = [](int i, double a, double b)
        {
            return i * a * b;
        };

        CHECK(map_with_index(product, array_3, vector_3) == Vector<double>{0., 4., 18.});
    }
}

TEST_CASE("cartesian_map")
{
    auto as = Array<int, 2>{1, 2};
    auto bs = Array<int, 2>{1, 3};

    auto res = cartesian_map([](int a, int b)
                             { return a * b; },
                             as, bs);
    CHECK(res == Array<int, 4>{1, 3, 2, 6});
}

TEST_CASE("cartesian_for_index")
{
    Vector<int> res;
    const auto f = [&](int a, int b)
    { return res.push_back(a * b); };

    cartesian_for_index(f, 2, 3);
    CHECK(res == Vector<int>{0, 0, 0, 0, 1, 2});
    // Array<int, -2> invalid{};
}

TEST_CASE("head")
{
    CHECK(head(array_3) == 1);
    CHECK(head(arrvec_3) == 1);
    CHECK(head(vector_3) == 1);
}

TEST_CASE("tail")
{
    SECTION("const")
    {
        CHECK(tail(array_3) == ArrayView<const double, 2>{data(array_3) + 1});
        CHECK(tail(arrvec_3) == ArrVecView<const double, 2>{data(arrvec_3) + 1, 2});
        CHECK(tail(vector_3) == VectorView<const double>{data(vector_3) + 1, 2, 2});
    }

    SECTION("non const")
    {
        Array<double, 3> array{1., 2., 3.};
        ArrVec<double, 3> arrvec{1., 2., 3.};
        Vector<double> vector{1., 2., 3.};

        CHECK(tail(array) == ArrayView<double, 2>{data(array) + 1});
        CHECK(tail(arrvec) == ArrVecView<double, 2>{data(arrvec) + 1, 2});
        CHECK(tail(vector) == VectorView<double>{data(vector) + 1, 2, 2});
    }
}

TEST_CASE("init")
{
    SECTION("const")
    {
        CHECK(init(array_3) == ArrayView<const double, 2>{data(array_3)});
        CHECK(init(arrvec_3) == ArrVecView<const double, 2>{data(arrvec_3), 2});
        CHECK(init(vector_3) == VectorView<const double>{data(vector_3), 2, 2});
    }

    SECTION("non const")
    {
        Array<double, 3> array{1., 2., 3.};
        ArrVec<double, 3> arrvec{1., 2., 3.};
        Vector<double> vector{1., 2., 3.};

        CHECK(init(array) == ArrayView<double, 2>{data(array)});
        CHECK(init(arrvec) == ArrVecView<double, 2>{data(arrvec), 2});
        CHECK(init(vector) == VectorView<double>{data(vector), 2, 2});
    }
}

TEST_CASE("last")
{
    CHECK(last(array_3) == 3);
    CHECK(last(arrvec_3) == 3);
    CHECK(last(vector_3) == 3);
}

TEST_CASE("take")
{
    SECTION("array 0")
    {
        const auto res = take(2, array_3);
        CHECK(data(res) == data(array_3));
        CHECK(length(res) == 2);
        CHECK(res[0] == 1.);
        CHECK(res[1] == 2.);
    }

    SECTION("array 1")
    {
        const auto res = take(-1, array_3);
        CHECK(data(res) == data(array_3));
        CHECK(length(res) == 0);
    }

    SECTION("array 2")
    {
        const auto res = take(9, array_3);
        CHECK(data(res) == data(array_3));
        CHECK(length(res) == 3);
        CHECK(res[0] == 1.);
        CHECK(res[1] == 2.);
        CHECK(res[2] == 3.);
    }
}

TEST_CASE("drop")
{
    SECTION("array 0")
    {
        const auto res = drop(2, array_3);
        CHECK(data(res) == data(array_3) + 2);
        CHECK(length(res) == 1);
        CHECK(res[0] == 3.);
    }

    SECTION("array 1")
    {
        const auto res = drop(-1, array_3);
        CHECK(data(res) == data(array_3));
        CHECK(length(res) == 3);
        CHECK(res[0] == 1.);
        CHECK(res[1] == 2.);
        CHECK(res[2] == 3.);
    }

    SECTION("array 2")
    {
        const auto res = drop(9, array_3);
        CHECK(data(res) == data(array_3) + 3);
        CHECK(length(res) == 0);
    }
}

TEST_CASE("slice")
{
    SECTION("static")
    {
        const auto slice_ = slice(Int<1>{}, Int<3>{}, array_5);
        CHECK(length(slice_) == 2);
        CHECK(IsStaticLength<decltype(slice_)>::value);
        CHECK(slice_[0] == 2.);
        CHECK(slice_[1] == 3.);
    }

    SECTION("dynamic")
    {
        const auto slice_ = slice(1, 3, array_5);
        CHECK(IsSame<decltype(length(slice_)), int>::value);
        CHECK(!IsStaticLength<decltype(slice_)>::value);
        CHECK(slice_[0] == 2.);
        CHECK(slice_[1] == 3.);
    }
}

TEST_CASE("elem")
{
    SECTION("Array")
    {
        CHECK(elem(2., array_3) == true);
        CHECK(elem(9., array_3) == false);
    }

    SECTION("ArrVec")
    {
        CHECK(elem(2., arrvec_3) == true);
        CHECK(elem(9., arrvec_3) == false);
    }

    SECTION("Vector")
    {
        CHECK(elem(2., vector_3) == true);
        CHECK(elem(9., vector_3) == false);
    }
}

TEST_CASE("elem_index")
{
    SECTION("Array")
    {
        CHECK(elem_index(2., array_3).value() == 1);
        CHECK(elem_index(9., array_3).is_nothing() == true);
    }

    SECTION("ArrVec")
    {
        CHECK(elem_index(2., arrvec_3).value() == 1);
        CHECK(elem_index(9., arrvec_3).is_nothing() == true);
    }

    SECTION("ArrayView")
    {
        CHECK(elem_index(2., array_view_3).value() == 1);
        CHECK(elem_index(9., array_view_3).is_nothing() == true);
    }
}

TEST_CASE("elem_indices")
{
    SECTION("Array")
    {
        const Array<double, 3> array_3_ = {1., 2., 2.};
        CHECK(elem_indices(2., array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(elem_indices(9., array_3_) == ArrVec<int, 3>{});
    }

    SECTION("ArrVec")
    {
        const ArrVec<double, 3> arrvec_3_ = {1., 2., 2.};
        CHECK(elem_indices(2., arrvec_3_) == ArrVec<int, 3>{1, 2});
        CHECK(elem_indices(9., arrvec_3_) == ArrVec<int, 3>{});
    }

    SECTION("Vector")
    {
        Vector<double> vector_3_ = {1, 2, 2};
        CHECK(elem_indices(2., vector_3_) == Vector<int>{1, 2});
        CHECK(elem_indices(9., vector_3_) == Vector<int>{});
    }
}

TEST_CASE("find")
{
    auto is_two = [](double x)
    { return x == 2. ? true : false; };

    auto is_nine = [](double x)
    { return x == 9. ? true : false; };

    SECTION("Array")
    {
        CHECK(find(is_two, array_3) == true);
        CHECK(find(is_nine, array_3) == false);
    }

    SECTION("Vector")
    {
        CHECK(find(is_two, vector_3) == true);
        CHECK(find(is_nine, vector_3) == false);
    }

    SECTION("ArrayView")
    {
        CHECK(find(is_two, array_view_3) == true);
        CHECK(find(is_nine, array_view_3) == false);
    }
}

TEST_CASE("find_index")
{
    auto is_two = [](double x)
    { return x == 2. ? true : false; };

    auto is_nine = [](double x)
    { return x == 9. ? true : false; };

    SECTION("Array")
    {
        CHECK(find_index(is_two, array_3).value() == 1);
        CHECK(find_index(is_nine, array_3).is_nothing() == true);
    }

    SECTION("Vector")
    {
        CHECK(find_index(is_two, vector_3).value() == 1);
        CHECK(find_index(is_nine, vector_3).is_nothing() == true);
    }

    SECTION("ArrayView")
    {
        CHECK(find_index(is_two, array_view_3).value() == 1);
        CHECK(find_index(is_nine, array_view_3).is_nothing() == true);
    }
}

TEST_CASE("find_indices")
{
    auto is_two = [](double x)
    { return x == 2. ? true : false; };

    auto is_nine = [](double x)
    { return x == 9. ? true : false; };

    SECTION("Array")
    {
        const Array<double, 3> array_3_ = {1., 2., 2.};
        CHECK(find_indices(is_two, array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(find_indices(is_nine, array_3_) == ArrVec<int, 3>{});
    }

    SECTION("ArrVec")
    {
        const ArrVec<double, 3> arrvec_3_{1., 2., 2.};
        CHECK(find_indices(is_two, arrvec_3_) == ArrVec<int, 3>{1, 2});
        CHECK(find_indices(is_nine, arrvec_3_) == ArrVec<int, 3>{});
    }

    SECTION("Vector")
    {
        const Vector<double> vector_3_{1., 2., 2.};
        CHECK(find_indices(is_two, vector_3_) == Vector<int>{1, 2});
        CHECK(find_indices(is_nine, vector_3_) == Vector<int>{});
    }
}

#endif