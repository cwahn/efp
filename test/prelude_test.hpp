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
}

TEST_CASE("map")
{
    SECTION("c style array")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        CHECK(map(square, c_array_3) == std::array<int, 3>{1, 4, 9});
    }

    SECTION("std::vector")
    {
        auto square = [](int x)
        {
            return x * x;
        };

        CHECK(map(square, std_vector_3) == std::vector<int>{1, 4, 9});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        std::vector<int> ref = {2, 4, 6};

        CHECK(map(plus, std_array_3, std_vector_3) == std::vector<int>{2, 4, 6});
    }
}

TEST_CASE("filter")
{
    SECTION("even c style array")
    {
        auto is_even = [](int x)
        {
            return x % 2 == 0;
        };

        CHECK(filter(is_even, c_array_3) == std::vector<int>{2});
    }

    SECTION("odd std::vector")
    {
        auto is_odd = [](int x)
        {
            return x % 2 != 0;
        };

        CHECK(filter(is_odd, std_vector_3) == std::vector<double>{1, 3});
    }
}

TEST_CASE("foldl")
{
    SECTION("plus on c style arary")
    {
        auto plus = [](int lhs, int rhs)
        {
            return lhs + rhs;
        };
        CHECK(foldl(plus, 0, c_array_5) == 15);
    }

    SECTION("times on std::vectors arary")
    {
        auto times = [](double lhs, double rhs)
        {
            return lhs * rhs;
        };
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
    SECTION("StaticArray")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(StaticSizeT<3>{}, plus_one) == StaticArray<int, 3>{1, 2, 3});
    }

    SECTION("DynamicArray")
    {
        auto plus_one = [](int i)
        {
            return i + 1;
        };

        CHECK(from_function(3, plus_one) == DynamicVector<int>{1, 2, 3});
    }
}

TEST_CASE("for_each_with_index")
{
    StaticArray<int, 3> res;

    auto product = [&](int i, int x0, int x1)
    {
        res[i] = x0 * x1;
    };

    for_each_with_index(product, c_array_3, std_vector_5);
    CHECK(res == std::array<int, 3>{1, 4, 9});
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

        CHECK(map_with_index(times, c_array_3) == std::array<int, 3>{0, 2, 6});
    }

    SECTION("std::vector")
    {
        auto times = [](int lhs, int rhs)
        {
            return lhs * rhs;
        };

        CHECK(map_with_index(times, std_vector_3) == DynamicVector<int>{0, 2, 6});
    }

    SECTION("binary of std::array, std::vecor")
    {
        auto product = [](int i, int a, int b)
        {
            return i * a * b;
        };

        CHECK(map_with_index(product, std_array_3, std_vector_3) == DynamicVector<int>{0, 4, 18});
    }
}

TEST_CASE("cartesian_map")
{
    auto as = std::array<int, 2>{1, 2};
    auto bs = std::array<int, 2>{1, 3};

    auto res = cartesian_map(times<int>, as, bs);
    CHECK(res == StaticArray<int, 4>{1, 3, 2, 6});
}

// ! temp

TEST_CASE("IsCallOperator")
{
    struct No
    {
    };

    struct Yes
    {
        void operator()(int x){};
    };

    auto lambda = [](int x) {};

    CHECK(IsCallOperator<No>::value == false);
    CHECK(IsCallOperator<Yes>::value == true);
    CHECK(IsCallOperator<decltype(lambda)>::value == true);
}

void argument_t_function0(int x0, float &&x1)
{
}

void argument_t_function1(const int x0, float &x1)
{
}

TEST_CASE("Argument_t")
{
    auto argument_t_lambda0 = [](int x0, float &&x1) {};
    auto argument_t_lambda1 = [](const int x0, float &x1) {};

    CHECK(std::is_same<
              std::tuple<int, float>,
              Argument_t<decltype(&argument_t_function0)>>::value == false);

    // Can catch l-value reference r-value reference
    CHECK(std::is_same<
              std::tuple<int, float &&>,
              Argument_t<decltype(&argument_t_function0)>>::value == true);

    // Can't catch const qualifier
    CHECK(std::is_same<
              std::tuple<const int, float &>,
              Argument_t<decltype(&argument_t_function1)>>::value == false);

    CHECK(std::is_same<
              std::tuple<int, float &>,
              Argument_t<decltype(&argument_t_function1)>>::value == true);

    CHECK(std::is_same<
              std::tuple<int, float>,
              Argument_t<decltype(argument_t_lambda0)>>::value == false);

    // Can catch l-value reference r-value reference
    CHECK(std::is_same<
              std::tuple<int, float &&>,
              Argument_t<decltype(argument_t_lambda0)>>::value == true);

    // Can't catch const qualifier
    CHECK(std::is_same<
              std::tuple<const int, float &>,
              Argument_t<decltype(argument_t_lambda1)>>::value == false);

    CHECK(std::is_same<
              std::tuple<int, float &>,
              Argument_t<decltype(argument_t_lambda1)>>::value == true);
}

double return_t_function0(int x0, float &&x1)
{
}

double *return_t_function1(const int x0, float &x1)
{
}

TEST_CASE("Return_t")
{
    auto return_t_lambda0 = [](int x0, float x1)
    { return x1 + x0; };
    auto return_t_lambda1 = [](const int x0, float &x1)
    { return x0 + x1; };

    CHECK(std::is_same<
              double,
              Return_t<decltype(&return_t_function0)>>::value == true);

    CHECK(std::is_same<
              double &,
              Return_t<decltype(&return_t_function0)>>::value == false);

    CHECK(std::is_same<
              double *,
              Return_t<decltype(&return_t_function1)>>::value == true);

    CHECK(std::is_same<
              double,
              Return_t<decltype(&return_t_function1)>>::value == false);

    CHECK(std::is_same<
              float,
              Return_t<decltype(return_t_lambda0)>>::value == true);

    CHECK(std::is_same<
              int &,
              Return_t<decltype(return_t_lambda0)>>::value == false);

    CHECK(std::is_same<
              float,
              Return_t<decltype(return_t_lambda1)>>::value == true);

    CHECK(std::is_same<
              float *,
              Return_t<decltype(return_t_lambda1)>>::value == false);
}

static void (*inner_function)(void *) = nullptr;

static void outer_function()
{
    if (inner_function != nullptr)
    {
        inner_function(nullptr);
    }
}

static void (*inner_function2)(void *) = nullptr;

static void outer_function2()
{
    if (inner_function2 != nullptr)
    {
        inner_function2(nullptr);
    }
}

TEST_CASE("to_function_pointer")
{
    int a = 100;
    auto b = [&](void *)
    { return ++a; };

    void (*f1)(void *) = Lambda::ptr(b);
    f1(nullptr);
    CHECK(a == 101);

    outer_function();
    CHECK(a == 101);

    inner_function = Lambda::ptr(b);

    outer_function();
    CHECK(a == 102);

    outer_function();
    CHECK(a == 103);

    auto lambda1 = [](int x1, float x2) -> double
    {
        return 1.;
    };
}

#endif