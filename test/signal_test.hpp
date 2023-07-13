#ifndef SIGNAL_TEST_
#define SIGNAL_TEST_

#include "catch2/catch_test_macros.hpp"

#include "test_common.hpp"
#include "signal.hpp"

double abs_error(double x_hat, double x)
{
    return abs(x_hat - x) / x;
}

TEST_CASE("sse")
{
    SECTION("c style")
    {
        int zeros[3] = {0, 0, 0};
        CHECK(sse(c_array_3, c_array_3) == 0);
        CHECK(sse(c_array_3, zeros) == 14);
    }

    SECTION("std::array")
    {
        std::array<double, 3> zeros = {0., 0., 0.};
        CHECK(sse(std_array_3, std_array_3) == 0.);
        CHECK(sse(std_array_3, zeros) == 14.);
    }

    SECTION("std::vectors")
    {
        std::vector<double> zeros = {0., 0., 0.};
        CHECK(sse(std_vector_3, std_vector_3) == 0.);
        CHECK(sse(std_vector_3, zeros) == 14.);
    }
}

TEST_CASE("mse")
{
    SECTION("c style")
    {
        int zeros[3] = {0, 0, 0};
        CHECK(mse(c_array_3, c_array_3) == 0);
        CHECK(mse(c_array_3, zeros) == 14 / 3.);
    }

    SECTION("std::array")
    {
        std::array<double, 3> zeros = {0., 0., 0.};
        CHECK(mse(std_array_3, std_array_3) == 0.);
        CHECK(mse(std_array_3, zeros) == 14. / 3.);
    }

    SECTION("std::vectors")
    {
        std::vector<double> zeros = {0., 0., 0.};
        CHECK(mse(std_vector_3, std_vector_3) == 0.);
        CHECK(mse(std_vector_3, zeros) == 14. / 3.);
    }
}

TEST_CASE("rms")
{
    SECTION("c style")
    {
        int zeros[3] = {0, 0, 0};
        CHECK(rms(c_array_3, c_array_3) == 0);
        CHECK(rms(c_array_3, zeros) == sqrt(14 / 3.));
    }

    SECTION("std::array")
    {
        std::array<double, 3> zeros = {0., 0., 0.};
        CHECK(rms(std_array_3, std_array_3) == 0.);
        CHECK(rms(std_array_3, zeros) == sqrt(14. / 3.));
    }

    SECTION("std::vectors")
    {
        std::vector<double> zeros = {0., 0., 0.};
        CHECK(rms(std_vector_3, std_vector_3) == 0.);
        CHECK(rms(std_vector_3, zeros) == sqrt(14. / 3.));
    }
}

TEST_CASE("nrms")
{
    SECTION("c style")
    {
        int twos[3] = {2, 2, 2};
        CHECK(nrms_mean(c_array_3, c_array_3) == 0);
        CHECK(nrms_mean(c_array_3, twos) == sqrt(2 / 3.) / 2.);
    }

    SECTION("std::array")
    {
        std::array<double, 3> twos = {2., 2., 2.};
        CHECK(nrms_mean(std_array_3, std_array_3) == 0.);
        CHECK(nrms_mean(std_array_3, twos) == sqrt(2. / 3.) / 2.);
    }

    SECTION("std::vectors")
    {
        std::vector<double> twos = {2., 2., 2.};
        CHECK(nrms_mean(std_vector_3, std_vector_3) == 0.);
        CHECK(nrms_mean(std_vector_3, twos) == sqrt(2. / 3.) / 2.);
    }
}

TEST_CASE("remove_dc")
{

    constexpr size_t n = 100;

    double a = 2;
    double b = 100;

    auto f = [&](double x)
    {
        return a * x + b;
    };

    auto f_ref = [&](double x)
    {
        return a * (x - n / 2);
    };

    SECTION("c style")
    {
        double xs[n];
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc(ys);

        CHECK(nrms_max_min(dc_removed, ys_ref) < 0.05);
    }

    SECTION("std::array")
    {
        std::array<double, n> xs;
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc(ys);

        CHECK(nrms_max_min(dc_removed, ys_ref) < 0.05);
    }

    SECTION("std::vector")
    {
        std::vector<double> xs(n);
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc(ys);

        CHECK(nrms_max_min(dc_removed, ys_ref) < 0.05);
    }
}

TEST_CASE("linear_regression")
{

    constexpr size_t n = 100;

    double a = 2;
    double b = 100;

    auto f = [&](double x)
    {
        return sin(x * 2 * M_PI / n) + a * x + b;
    };

    SECTION("StaticArray")
    {
        auto xs = from_function<StaticSizeT<n>{}, id<int>>;

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression(xs, ys);

        // CHECK(b_hat == b);
        // CHECK(a_hat == a);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("c style")
    {
        double xs[n];
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression(std_array_xs, ys);

        // CHECK(b_hat == b);
        // CHECK(a_hat == a);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::array")
    {
        std::array<double, n> xs;
        std::iota(std::begin(xs), std::end(xs), 0);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression(xs, ys);

        // CHECK(b_hat == b);
        // CHECK(a_hat == a);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::vector")
    {
        std::vector<double> xs(n);
        std::iota(std::begin(xs), std::end(xs), 0);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression(xs, ys);

        // CHECK(b_hat == b);
        // CHECK(a_hat == a);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }
}

// TEST_CASE("linear_regression_with_index")
// {

//     constexpr size_t n = 100;

//     double a = 2;
//     double b = 100;

//     auto f = [&](double x)
//     {
//         return sin(x * 2 * M_PI / n) + a * x + b;
//     };

//     SECTION("c style")
//     {
//         auto xs = map_with_index([](x)
//                                  { return x }) auto ys = map_with_index(f);

//         double a_hat, b_hat;
//         std::tie(a_hat, b_hat) = linear_regression(std_array_xs, ys);

//         // CHECK(b_hat == b);
//         // CHECK(a_hat == a);

//         CHECK(abs_error(a_hat, a) < 0.05);
//         CHECK(abs_error(b_hat, b) < 0.05);
//     }

//     SECTION("std::array")
//     {
//         std::array<double, n> xs;
//         std::iota(std::begin(xs), std::end(xs), 0);

//         auto ys = map(f, xs);

//         double a_hat, b_hat;
//         std::tie(a_hat, b_hat) = linear_regression(xs, ys);

//         // CHECK(b_hat == b);
//         // CHECK(a_hat == a);

//         CHECK(abs_error(a_hat, a) < 0.05);
//         CHECK(abs_error(b_hat, b) < 0.05);
//     }

//     SECTION("std::vector")
//     {
//         std::vector<double> xs(n);
//         std::iota(std::begin(xs), std::end(xs), 0);

//         auto ys = map(f, xs);

//         double a_hat, b_hat;
//         std::tie(a_hat, b_hat) = linear_regression(xs, ys);

//         // CHECK(b_hat == b);
//         // CHECK(a_hat == a);

//         CHECK(abs_error(a_hat, a) < 0.05);
//         CHECK(abs_error(b_hat, b) < 0.05);
//     }
// }

TEST_CASE("detrend")
{
    constexpr size_t n = 100;

    double a = 2;
    double b = 100;

    auto f1 = [&](double x)
    {
        return a * x + b;
    };

    auto f2 = [](double x)
    {
        return sin(2 * M_PI * x / n);
    };

    SECTION("c style")
    {
        double xs[n];
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys = map(f1, xs);

        std::vector<double> ys_ref(n);
        std::fill(std::begin(ys_ref), std::end(ys_ref), 0);

        auto detrended = detrend(ys);

        CHECK(rms(detrended, ys_ref) < 0.5);
    }

    SECTION("std::array")
    {
        // auto xs = from_function(StaticSizeT<n>{}, id<int>);
        auto xs = from_function(n, id<int>);

        auto ys1 = map(f1, xs);
        auto ys2 = map(f2, xs);

        double ys_ref[n];
        std::fill(std::begin(ys_ref), std::end(ys_ref), 0);

        auto ys = map(plus<double>, ys1, ys2);

        auto detrended = detrend(ys);

        CHECK(rms(detrended, ys_ref) < 0.5);
    }

    SECTION("std::vectors")
    {
        double xs[n];
        std::iota(std::begin(xs), std::end(xs), 0);

        std::array<double, n> std_array_xs;
        std::copy(std::begin(xs), std::end(xs), std::begin(std_array_xs));

        auto ys1 = map(f1, xs);
        auto ys2 = map(f2, xs);

        std::array<double, n> ys_ref;
        std::fill(std::begin(ys_ref), std::end(ys_ref), 0);

        auto ys = map(plus<double>, ys1, ys2);

        auto detrended = detrend(ys);

        CHECK(rms(detrended, ys_ref) < 0.5);
    }
}

#endif