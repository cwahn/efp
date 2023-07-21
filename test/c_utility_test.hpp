#ifndef C_UTILITY_TEST_HPP_
#define C_UTILITY_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "test_common.hpp"
#include "efp.hpp"

using namespace efp;

double fpt_function0(int x0, float &&x1)
{
    return 0.;
}

double *fpt_function1(const int x0, float &x1)
{
    return nullptr;
}

TEST_CASE("FunctionPointer_t")
{
    auto fpt_lambda0 = [](int x0, float x1)
    { return x1 + x0; };
    auto fpt_lambda1 = [](const int x0, float &x1)
    { return x0 + x1; };

    CHECK(std::is_same<
              double (*)(int, float &&),
              FunctionPointer_t<decltype(&fpt_function0)>>::value == true);

    CHECK(std::is_same<
              double (*)(int, float),
              FunctionPointer_t<decltype(&fpt_function0)>>::value == false);

    CHECK(std::is_same<
              double *(*)(int, float &),
              FunctionPointer_t<decltype(&fpt_function1)>>::value == true);

    CHECK(std::is_same<
              double *(*)(int, float),
              FunctionPointer_t<decltype(&fpt_function1)>>::value == false);

    CHECK(std::is_same<
              float (*)(int, float),
              FunctionPointer_t<decltype(fpt_lambda0)>>::value == true);

    CHECK(std::is_same<
              double (*)(int, float),
              FunctionPointer_t<decltype(fpt_lambda0)>>::value == false);

    CHECK(std::is_same<
              float (*)(int, float &),
              FunctionPointer_t<decltype(fpt_lambda1)>>::value == true);

    CHECK(std::is_same<
              float (*)(int, float),
              FunctionPointer_t<decltype(fpt_lambda1)>>::value == false);
}

static void (*inner_function1)(void *) = nullptr;

static void outer_function1()
{
    if (inner_function1 != nullptr)
    {
        inner_function1(nullptr);
    }
}

static double (*inner_function2)(int, float);

static void outer_function2(int x0, float x1)
{
    if (inner_function2 != nullptr)
    {
        inner_function2(x0, x1);
    }
}

static void register_callback(double (*fptr)(int, float))
{
    inner_function2 = fptr;
}

TEST_CASE("to_function_pointer")
{
    SECTION("no_argument")
    {
        int a = 100;
        auto b = [&](void *)
        { ++a; };

        void (*f1)(void *) = to_function_pointer(b);
        f1(nullptr);
        CHECK(a == 101);

        outer_function1();
        CHECK(a == 101);

        inner_function1 = to_function_pointer(b);

        outer_function1();
        CHECK(a == 102);

        outer_function1();
        CHECK(a == 103);
    }

    SECTION("general")
    {
        double a = 0.;

        static auto b = [&](int x0, float x1)
        {
            a += x0 + x1;
            return a * 2;
        };

        // ! Currently accessing before callback registration is segment violation.
        // todo Need to set default value

        // inner_function2(1, 2.);
        // CHECK(a == 0.);

        register_callback(to_function_pointer(b));

        CHECK(inner_function2(1, 2.) == 6.);
        CHECK(a == 3.);

        outer_function2(1, 2.);
        CHECK(a == 6.);
    }
}

#endif