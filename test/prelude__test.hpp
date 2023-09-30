#ifndef PRELUDE_TEST_HPP_
#define PRELUDE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

// ! WIP

TEST_CASE("compose")
{
    SECTION("pure0")
    {
        auto add_1 = [](int x)
        {
            return x + 1;
        };

        auto times_2 = [](int x)
        {
            return x * 2;
        };

        auto composed = compose(add_1, times_2);
        // auto composed_with_operator = add_1 * times_2;

        CHECK(composed(2) == 5);
        // CHECK(composed_with_operator(2) == 5);
    }

    SECTION("pure1")
    {
        auto add_1 = [](int x)
        {
            return x + 1;
        };

        auto devide_2 = [](int x)
        {
            return x / 2.;
        };

        auto composed = compose(devide_2, add_1);
        CHECK(composed(2) == 1.5);
    }

    SECTION("side-effect")
    {
        int i = 0;

        auto i_plusplus = [&](int x)
        {
            i += x;
            return i;
        };

        auto times_2 = [](int x)
        {
            return x * 2;
        };

        auto composed = compose(times_2, i_plusplus);
        CHECK(composed(2) == 4);
        CHECK(i == 2);
    }

    // SECTION("n-ary")
    // {
    //     auto add_1 = [](int x)
    //     {
    //         return x + 1;
    //     };

    //     auto devide_2 = [](int x)
    //     {
    //         return x / 2.;
    //     };

    //     auto composed = compose(devide_2, add_1, square<int>);
    //     CHECK(composed(2) == 2.5);
    // }
}

// TEST_CASE("IsStaticCapacity")
// {
//     CHECK(IsStaticCapacity<int[3]>::value == true);
//     CHECK(IsStaticCapacity<std::array<int, 3>>::value == true);
//     CHECK(IsStaticCapacity<std::vector<int>>::value == false);
// }

// TEST_CASE("AreAllStaticCapacity")
// {
//     CHECK(AreAllStaticCapacity<int[3]>::value == true);
//     CHECK(AreAllStaticCapacity<std::array<int, 3>>::value == true);
//     CHECK(AreAllStaticCapacity<int[3], std::array<int, 3>>::value == true);
//     CHECK(AreAllStaticCapacity<std::vector<int>>::value == false);
//     CHECK(AreAllStaticCapacity<std::array<int, 3>, std::vector<int>>::value == false);
// }

TEST_CASE("append")
{
    SECTION("Array")
    {
        Array<double, 6> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(std_array_3, array_3) == res);
    }

    SECTION("ArrVec")
    {
        ArrVec<double, 6> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(array_vector_3, array_3) == res);
    }

    SECTION("Array")
    {
        Vector<double> res{1., 2., 3., 1., 2., 3.};
        CHECK(append(std_array_3, vector_3) == res);
    }
}

TEST_CASE("for_each")
{
    int res_1 = 0;
    auto add_1 = [&](int x)
    {
        res_1 += x;
    };

    for_each(add_1, c_array_3);
    CHECK(res_1 == 6);

    double res_2 = 0.;
    auto add_2 = [&](double x)
    {
        res_2 += x;
    };

    for_each(add_2, std_array_3);
    CHECK(res_2 == 6.);

    double res_3 = 0.;
    auto add_3 = [&](double x)
    {
        res_3 += x;
    };

    for_each(add_3, std_vector_3);
    CHECK(res_3 == 6.);

    double res_4 = 0.;
    auto add_product = [&](double x1, double x2)
    {
        res_4 += x1 * x2;
    };

    for_each(add_product, std_vector_3, c_array_3);
    CHECK(res_4 == 14.);

    double res_5 = 0.;

    auto add_product_2 = [&](double x1, double x2)
    {
        res_5 += x1 * x2;
    };

    for_each(add_product_2, vector_view_3, vector_view_5);
    CHECK(res_5 == 14.);
}

TEST_CASE("for_eachi")
{
    int res_1[3];
    auto set_42 = [](int &x)
    {
        x = 42;
    };

    for_eachi(set_42, res_1);

    const auto is_42 = [](int x)
    { return x == 42; };

    CHECK(foldl(and_v, true, map(is_42, res_1)) == true);

    std::array<int, 3> res_2;

    for_eachi(set_42, res_2);
    CHECK(res_2 == std::array<int, 3>{42, 42, 42});

    Vector<int> res_3{0, 0, 0};

    for_eachi(set_42, res_3);
    CHECK(res_3 == Vector<int>{42, 42, 42});

    SECTION("ArrayView")
    {
        int c_arr[3] = {0, 1, 2};
        ArrayView<int, 3> arr_view{&c_arr[0]};

        for_eachi([](int &x)
                  { x = 2 * x; },
                  arr_view);

        CHECK((c_arr[0] == 0 && c_arr[1] == 2 && c_arr[2] == 4));
    }
}

TEST_CASE("map")
{
    SECTION("c style array")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        CHECK(map(square, c_array_3) == Array<int, 3>{1, 4, 9});
    }

    SECTION("std::vector")
    {
        auto square = [](double x)
        {
            return x * x;
        };

        CHECK(map(square, std_vector_3) == Vector<double>{1., 4., 9.});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto plus = [](double lhs, double rhs)
        {
            return lhs + rhs;
        };

        CHECK(map(plus, std_array_3, std_vector_3) == Vector<double>{2., 4., 6.});
    }
}

TEST_CASE("filter")
{
    SECTION("even c style array")
    {
        auto is_even = [](double x)
        {
            return (int)x % 2 == 0;
        };

        CHECK(filter(is_even, c_array_3) == ArrVec<double, 3>{2.});
    }

    SECTION("odd std::vector")
    {
        auto is_odd = [](double x)
        {
            return (int)x % 2 != 0;
        };

        CHECK(filter(is_odd, std_vector_3) == Vector<double>{1., 3.});
    }
}

TEST_CASE("foldl")
{
    SECTION("plus on c style arary")
    {
        CHECK(foldl(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        CHECK(foldl(times, 1., std_vector_3) == 6.);
    }
}

TEST_CASE("foldr")
{
    SECTION("plus on c style arary")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        CHECK(foldr(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
        CHECK(foldr(times, 1., std_vector_3) == 6.);
    }
}

TEST_CASE("from_function")
{
    SECTION("Array")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(IntegralConst<int, 3>{}, plus_one) == Array<int, 3>{1, 2, 3});
    }

    SECTION("DynamicArray")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(3, plus_one) == Vector<int>{1, 2, 3});
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

    for_each_with_index(product, c_array_3, std_vector_5);
    CHECK(res == Array<int, 3>{1, 4, 9});
}

TEST_CASE("cartesian_for_each")
{
    std::vector<int> res;
    auto as = std::array<int, 2>{1, 2};
    auto bs = std::array<int, 2>{1, 3};

    auto push_res = [&](int x0, int x1)
    {
        res.push_back(x0 * x1);
    };

    cartesian_for_each(push_res, as, bs);
    CHECK(res == std::vector<int>{1, 3, 2, 6});
}

TEST_CASE("map_with_index")
{
    SECTION("c style array")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, c_array_3) == Array<int, 3>{0, 2, 6});
    }

    SECTION("std::vector")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, std_vector_3) == Vector<double>{0., 2., 6.});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto product = [](int i, double a, double b)
        {
            return i * a * b;
        };

        CHECK(map_with_index(product, std_array_3, std_vector_3) == Vector<double>{0., 4., 18.});
    }
}

TEST_CASE("cartesian_map")
{
    auto as = std::array<int, 2>{1, 2};
    auto bs = std::array<int, 2>{1, 3};

    auto res = cartesian_map(times<int, int>, as, bs);
    CHECK(res == Array<int, 4>{1, 3, 2, 6});
}

TEST_CASE("cartesian_for_index")
{
    Vector<int> res;
    const auto f = [&](int a, int b)
    { return res.push_back(a * b); };

    cartesian_for_index(f, 2, 3);
    CHECK(res == Vector<int>{0, 0, 0, 0, 1, 2});
}

// todo sequence view

TEST_CASE("head")
{
    auto std_array = std::array<int, 3>{1, 2, 3};
    auto std_vector = std::vector<int>{1, 2, 3};
    auto array = Array<int, 3>{1, 2, 3};
    auto arrvec = ArrVec<int, 3>{1, 2, 3};
    auto vector = Vector<int>{1, 2, 3};

    CHECK(head(std_array) == 1);
    CHECK(head(std_vector) == 1);
    CHECK(head(array) == 1);
    CHECK(head(arrvec) == 1);
    CHECK(head(vector) == 1);
}

TEST_CASE("tail")
{
    SECTION("const")
    {
        const auto std_array = std::array<int, 3>{1, 2, 3};
        const auto std_vector = std::vector<int>{1, 2, 3};
        const auto array = Array<int, 3>{1, 2, 3};
        const auto arrvec = ArrVec<int, 3>{1, 2, 3};
        const auto vector = Vector<int>{1, 2, 3};

        CHECK(tail(std_array) == VectorView<const int>{p_data(std_array) + 1, length(std_array) - 1});
        CHECK(tail(std_vector) == VectorView<const int>{p_data(std_vector) + 1, length(std_vector) - 1});
        CHECK(tail(array) == VectorView<const int>{p_data(array) + 1, length(array) - 1});
        CHECK(tail(arrvec) == VectorView<const int>{p_data(arrvec) + 1, length(arrvec) - 1});
        CHECK(tail(vector) == VectorView<const int>{p_data(vector) + 1, length(vector) - 1});
    }

    SECTION("non const")
    {
        std::array<int, 3> std_array{1, 2, 3};
        std::vector<int> std_vector{1, 2, 3};
        Array<int, 3> array{1, 2, 3};
        ArrVec<int, 3> arrvec{1, 2, 3};
        Vector<int> vector{1, 2, 3};

        CHECK(tail(std_array) == VectorView<int>{p_data(std_array) + 1, length(std_array) - 1});
        CHECK(tail(std_vector) == VectorView<int>{p_data(std_vector) + 1, length(std_vector) - 1});
        CHECK(tail(array) == VectorView<int>{p_data(array) + 1, length(array) - 1});
        CHECK(tail(arrvec) == VectorView<int>{p_data(arrvec) + 1, length(arrvec) - 1});
        CHECK(tail(vector) == VectorView<int>{p_data(vector) + 1, length(vector) - 1});
    }
}

TEST_CASE("init")
{
    SECTION("const")
    {
        const auto std_array = std::array<int, 3>{1, 2, 3};
        const auto std_vector = std::vector<int>{1, 2, 3};
        const auto array = Array<int, 3>{1, 2, 3};
        const auto arrvec = ArrVec<int, 3>{1, 2, 3};
        const auto vector = Vector<int>{1, 2, 3};

        CHECK(init(std_array) == VectorView<const int>{p_data(std_array), length(std_array) - 1});
        CHECK(init(std_vector) == VectorView<const int>{p_data(std_vector), length(std_vector) - 1});
        CHECK(init(array) == VectorView<const int>{p_data(array), length(array) - 1});
        CHECK(init(arrvec) == VectorView<const int>{p_data(arrvec), length(arrvec) - 1});
        CHECK(init(vector) == VectorView<const int>{p_data(vector), length(vector) - 1});
    }

    SECTION("non const")
    {
        std::array<int, 3> std_array{1, 2, 3};
        std::vector<int> std_vector{1, 2, 3};
        Array<int, 3> array{1, 2, 3};
        ArrVec<int, 3> arrvec{1, 2, 3};
        Vector<int> vector{1, 2, 3};

        CHECK(init(std_array) == VectorView<int>{p_data(std_array), length(std_array) - 1});
        CHECK(init(std_vector) == VectorView<int>{p_data(std_vector), length(std_vector) - 1});
        CHECK(init(array) == VectorView<int>{p_data(array), length(array) - 1});
        CHECK(init(arrvec) == VectorView<int>{p_data(arrvec), length(arrvec) - 1});
        CHECK(init(vector) == VectorView<int>{p_data(vector), length(vector) - 1});
    }
}

TEST_CASE("last")
{
    auto std_array = std::array<int, 3>{1, 2, 3};
    auto std_vector = std::vector<int>{1, 2, 3};
    auto array = Array<int, 3>{1, 2, 3};
    auto arrvec = ArrVec<int, 3>{1, 2, 3};
    auto vector = Vector<int>{1, 2, 3};

    CHECK(last(std_array) == 3);
    CHECK(last(std_vector) == 3);
    CHECK(last(array) == 3);
    CHECK(last(arrvec) == 3);
    CHECK(last(vector) == 3);
}

TEST_CASE("take")
{
    SECTION("c array0")
    {
        const auto res = take(2, c_array_3);
        CHECK(p_data(res) == p_data(c_array_3));
        CHECK(length(res) == 2);
        CHECK(res[0] == 1.);
        CHECK(res[1] == 2.);
    }

    SECTION("c array1")
    {
        const auto res = take(-1, c_array_3);
        CHECK(p_data(res) == p_data(c_array_3));
        CHECK(length(res) == 0);
    }

    SECTION("c array2")
    {
        const auto res = take(9, c_array_3);
        CHECK(p_data(res) == p_data(c_array_3));
        CHECK(length(res) == 3);
        CHECK(res[0] == 1.);
        CHECK(res[1] == 2.);
        CHECK(res[2] == 3.);
    }
}

TEST_CASE("drop")
{
    SECTION("c array0")
    {
        const auto res = drop(2, c_array_3);
        CHECK(p_data(res) == p_data(c_array_3) + 2);
        CHECK(length(res) == 1);
        CHECK(res[0] == 3.);
    }

    SECTION("c array1")
    {
        const auto res = drop(-1, c_array_3);
        CHECK(p_data(res) == p_data(c_array_3));
        CHECK(length(res) == 3);
    }

    SECTION("c array2")
    {
        const auto res = drop(9, c_array_3);
        CHECK(p_data(res) == p_end(c_array_3));
        CHECK(length(res) == 0);
    }
}

TEST_CASE("elem")
{
    SECTION("c array")
    {
        CHECK(elem(2., c_array_3) == true);
        CHECK(elem(9., c_array_3) == false);
    }

    SECTION("Array")
    {
        CHECK(elem(2., array_3) == true);
        CHECK(elem(9., array_3) == false);
    }

    SECTION("ArrayView")
    {
        CHECK(elem(2., array_view_3) == true);
        CHECK(elem(9., array_view_3) == false);
    }
}

TEST_CASE("elem_index")
{
    SECTION("c array")
    {
        CHECK(elem_index(2., c_array_3).value() == 1);
        CHECK(elem_index(9., c_array_3).is_nothing() == true);
    }

    SECTION("Array")
    {
        CHECK(elem_index(2., array_3).value() == 1);
        CHECK(elem_index(9., array_3).is_nothing() == true);
    }

    SECTION("ArrayView")
    {
        CHECK(elem_index(2., array_view_3).value() == 1);
        CHECK(elem_index(9., array_view_3).is_nothing() == true);
    }
}

TEST_CASE("elem_indices")
{
    SECTION("c array")
    {
        double c_array_3_[3] = {1., 2., 2.};
        CHECK(elem_indices(2., c_array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(elem_indices(9., c_array_3_) == ArrVec<int, 3>{});
    }

    SECTION("Array")
    {
        double array_3_[3] = {1., 2., 2.};
        CHECK(elem_indices(2., array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(elem_indices(9., array_3_) == ArrVec<int, 3>{});
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

    SECTION("c array")
    {
        CHECK(find(is_two, c_array_3) == true);
        CHECK(find(is_nine, c_array_3) == false);
    }

    SECTION("Array")
    {
        CHECK(find(is_two, array_3) == true);
        CHECK(find(is_nine, array_3) == false);
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

    SECTION("c array")
    {
        CHECK(find_index(is_two, c_array_3).value() == 1);
        CHECK(find_index(is_nine, c_array_3).is_nothing() == true);
    }

    SECTION("Array")
    {
        CHECK(find_index(is_two, array_3).value() == 1);
        CHECK(find_index(is_nine, array_3).is_nothing() == true);
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

    SECTION("c array")
    {
        double c_array_3_[3] = {1., 2., 2.};
        CHECK(find_indices(is_two, c_array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(find_indices(is_nine, c_array_3_) == ArrVec<int, 3>{});
    }

    SECTION("Array")
    {
        double array_3_[3] = {1., 2., 2.};
        CHECK(find_indices(is_two, array_3_) == ArrVec<int, 3>{1, 2});
        CHECK(find_indices(is_nine, array_3_) == ArrVec<int, 3>{});
    }

    SECTION("Vector")
    {
        Vector<double> vector_3_ = {1, 2, 2};
        CHECK(find_indices(is_two, vector_3_) == Vector<int>{1, 2});
        CHECK(find_indices(is_nine, vector_3_) == Vector<int>{});
    }
}

#endif