#ifndef SCIENTIFIC_TEST_
#define SCIENTIFIC_TEST_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

double abs_error(double x_hat, double x)
{
    return abs(x_hat - x) / x;
}

TEST_CASE("mean")
{
    SECTION("Array")
    {
        CHECK(mean<double>(array_5) == 3.);
    }

    SECTION("ArrVec")
    {
        CHECK(mean<double>(arrvec_5) == 3.);
    }

    SECTION("Vector")
    {
        CHECK(mean<double>(vector_5) == 3.);
    }
}

TEST_CASE("rms")
{
    SECTION("Array")
    {
        CHECK(rms<double>(array_5) == 3.3166247903554);
    }

    SECTION("ArrVec")
    {
        CHECK(rms<double>(arrvec_5) == 3.3166247903554);
    }

    SECTION("Vector")
    {
        CHECK(rms<double>(vector_5) == 3.3166247903554);
    }
}

TEST_CASE("sse")
{
    SECTION("Array")
    {
        Array<int, 3> zeros = {0, 0, 0};
        CHECK(sse<double>(array_3, array_3) == 0);
        CHECK(sse<double>(array_3, zeros) == 14);
    }

    SECTION("ArrVec")
    {
        ArrVec<double, 3> zeros = {0., 0., 0.};
        CHECK(sse<double>(arrvec_3, arrvec_3) == 0.);
        CHECK(sse<double>(arrvec_3, zeros) == 14.);
    }

    SECTION("Vector")
    {
        Vector<double> zeros = {0., 0., 0.};
        CHECK(sse<double>(vector_3, vector_3) == 0.);
        CHECK(sse<double>(vector_3, zeros) == 14.);
    }
}

TEST_CASE("mse")
{
    SECTION("Array")
    {
        Array<int, 3> zeros = {0, 0, 0};
        CHECK(mse<double>(array_3, array_3) == 0);
        CHECK(mse<double>(array_3, zeros) == 14 / 3.);
    }

    SECTION("std::array")
    {
        ArrVec<double, 3> zeros = {0., 0., 0.};
        CHECK(mse<double>(arrvec_3, arrvec_3) == 0.);
        CHECK(mse<double>(arrvec_3, zeros) == 14. / 3.);
    }

    SECTION("Vector")
    {
        Vector<double> zeros = {0., 0., 0.};
        CHECK(mse<double>(vector_3, vector_3) == 0.);
        CHECK(mse<double>(vector_3, zeros) == 14. / 3.);
    }
}

TEST_CASE("rmse")
{
    SECTION("Array")
    {
        Array<int, 3> zeros = {0, 0, 0};
        CHECK(rmse<double>(array_3, array_3) == 0);
        CHECK(rmse<double>(array_3, zeros) == sqrt(14 / 3.));
    }

    SECTION("std::array")
    {
        ArrVec<double, 3> zeros = {0., 0., 0.};
        CHECK(rmse<double>(arrvec_3, arrvec_3) == 0.);
        CHECK(rmse<double>(arrvec_3, zeros) == sqrt(14. / 3.));
    }

    SECTION("Vector")
    {
        Vector<double> zeros = {0., 0., 0.};
        CHECK(rmse<double>(vector_3, vector_3) == 0.);
        CHECK(rmse<double>(vector_3, zeros) == sqrt(14. / 3.));
    }
}

TEST_CASE("nrms")
{
    SECTION("Array")
    {
        Array<int, 3> twos = {2, 2, 2};
        CHECK(nrmse_mean<double>(array_3, array_3) == 0);
        CHECK(nrmse_mean<double>(array_3, twos) == sqrt(2 / 3.) / 2.);
    }

    SECTION("std::array")
    {
        ArrVec<double, 3> twos = {2., 2., 2.};
        CHECK(nrmse_mean<double>(arrvec_3, arrvec_3) == 0.);
        CHECK(nrmse_mean<double>(arrvec_3, twos) == sqrt(2. / 3.) / 2.);
    }

    SECTION("Vector")
    {
        Vector<double> twos = {2., 2., 2.};
        CHECK(nrmse_mean<double>(vector_3, vector_3) == 0.);
        CHECK(nrmse_mean<double>(vector_3, twos) == sqrt(2. / 3.) / 2.);
    }
}

TEST_CASE("variance")
{
    SECTION("Array")
    {
        CHECK(variance<double>(array_3) == 0.6666666666666666);
    }

    SECTION("std::array")
    {
        CHECK(variance<double>(arrvec_3) == 0.6666666666666666);
    }

    SECTION("Vector")
    {
        CHECK(variance<double>(vector_3) == 0.6666666666666666);
    }
}

TEST_CASE("standard_deviation")
{
    SECTION("Array")
    {
        CHECK(standard_deviation<double>(array_3) == 0.816496580927726);
    }

    SECTION("std::array")
    {
        CHECK(standard_deviation<double>(arrvec_3) == 0.816496580927726);
    }

    SECTION("Vector")
    {
        CHECK(standard_deviation<double>(vector_3) == 0.816496580927726);
    }
}

TEST_CASE("covariance")
{
    Array<double, 3> ref = {-1., -2., -3.};

    SECTION("Array")
    {
        CHECK(covariance<double>(array_3, ref) == -0.6666666666666666);
    }

    SECTION("std::array")
    {
        CHECK(covariance<double>(arrvec_3, ref) == -0.6666666666666666);
    }

    SECTION("Vector")
    {
        CHECK(covariance<double>(vector_3, ref) == -0.6666666666666666);
    }
}

TEST_CASE("correlation")
{
    Array<double, 3> ref = {-1., -2., -3.};

    SECTION("Array")
    {
        CHECK(correlation<double>(array_3, ref) == -1.);
    }

    SECTION("std::array")
    {
        CHECK(correlation<double>(arrvec_3, ref) == -1.);
    }

    SECTION("Vector")
    {
        CHECK(correlation<double>(vector_3, ref) == -1.);
    }
}

TEST_CASE("autocovariance")
{
    SECTION("Array")
    {
        CHECK(autocovariance<double, false, false>(array_5, 2) == -0.2);
        CHECK(autocovariance<double, true, false>(array_5, 2) == -0.25);
        CHECK(autocovariance<double, false, true>(array_5, 2) == -0.3333333333333333);
        CHECK(autocovariance<double, true, true>(array_5, 2) == -0.5);
    }

    SECTION("std::array")
    {
        CHECK(autocovariance<double, false, false>(arrvec_5, 2) == -0.2);
        CHECK(autocovariance<double, true, false>(arrvec_5, 2) == -0.25);
        CHECK(autocovariance<double, false, true>(arrvec_5, 2) == -0.3333333333333333);
        CHECK(autocovariance<double, true, true>(arrvec_5, 2) == -0.5);
    }

    SECTION("Vector")
    {
        CHECK(autocovariance<double, false, false>(vector_5, 2) == -0.2);
        CHECK(autocovariance<double, true, false>(vector_5, 2) == -0.25);
        CHECK(autocovariance<double, false, true>(vector_5, 2) == -0.3333333333333333);
        CHECK(autocovariance<double, true, true>(vector_5, 2) == -0.5);
    }
}

TEST_CASE("autocorrelation")
{
    SECTION("Array")
    {
        CHECK(autocorrelation<double, false, false>(array_5, 2) == -0.1);
        CHECK(autocorrelation<double, true, false>(array_5, 2) == -0.1);
        CHECK(autocorrelation<double, false, true>(array_5, 2) == -0.16666666666666666);
        CHECK(autocorrelation<double, true, true>(array_5, 2) == -0.2);
    }

    SECTION("std::array")
    {
        CHECK(autocorrelation<double, false, false>(arrvec_5, 2) == -0.1);
        CHECK(autocorrelation<double, true, false>(arrvec_5, 2) == -0.1);
        CHECK(autocorrelation<double, false, true>(arrvec_5, 2) == -0.16666666666666666);
        CHECK(autocorrelation<double, true, true>(arrvec_5, 2) == -0.2);
    }

    SECTION("Vector")
    {
        CHECK(autocorrelation<double, false, false>(vector_5, 2) == -0.1);
        CHECK(autocorrelation<double, true, false>(vector_5, 2) == -0.1);
        CHECK(autocorrelation<double, false, true>(vector_5, 2) == -0.16666666666666666);
        CHECK(autocorrelation<double, true, true>(vector_5, 2) == -0.2);
    }
}

TEST_CASE("remove_dc")
{

    constexpr int n = 100;

    double a = 2;
    double b = 100;

    auto f = [&](double x)
    { return a * x + b; };

    auto f_ref = [&](double x)
    { return a * (x - n / 2); };

    SECTION("Array")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc<double>(ys);

        CHECK(nrmse_max_min<double>(dc_removed, ys_ref) < 0.05);
    }

    SECTION("std::array")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc<double>(ys);

        CHECK(nrmse_max_min<double>(dc_removed, ys_ref) < 0.05);
    }

    SECTION("std::vector")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);
        auto ys_ref = map(f_ref, xs);
        auto dc_removed = remove_dc<double>(ys);

        CHECK(nrmse_max_min<double>(dc_removed, ys_ref) < 0.05);
    }
}

TEST_CASE("linear_regression")
{
    constexpr int n = 100;

    double a = 2;
    double b = 100;

    auto f = [&](double x)
    {
        return sin(x * 2 * M_PI / n) + a * x + b;
    };

    SECTION("Array")
    {
        auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression<double>(xs, ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("Array")
    {

        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression<double>(xs, ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::array")
    {

        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression<double>(xs, ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::vector")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        auto ys = map(f, xs);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression<double>(xs, ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }
}

TEST_CASE("linear_regression_with_index")
{
    constexpr int n = 100;

    double a = 2;
    double b = 100;

    auto f = [&](double x)
    {
        return sin(x * 2 * M_PI / n) + a * x + b;
    };

    SECTION("Array")
    {
        Array<double, 3> ys{};
        for_each_with_indexi([&](int i, double &x)
                             { x = f(i); },
                             ys);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression_with_index<double>(ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::array")
    {
        auto ys = from_function(IntegralConst<int, n>{}, f);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression_with_index<double>(ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }

    SECTION("std::vector")
    {
        auto ys = from_function(n, f);

        double a_hat, b_hat;
        std::tie(a_hat, b_hat) = linear_regression_with_index<double>(ys);

        CHECK(abs_error(a_hat, a) < 0.05);
        CHECK(abs_error(b_hat, b) < 0.05);
    }
}

TEST_CASE("detrend")
{
    constexpr int n = 100;

    double a = 2;
    double b = 100;

    auto f1 = [&](double x)
    { return a * x + b; };

    auto f2 = [n](double x)
    { return sin(2 * M_PI * x / n); };

    SECTION("Array")
    {
        const auto xs = from_function(n, id<int>);
        
        const auto ys = map(f1, xs);

        const auto ys_ref = from_function(n, [](int _)
                                          { return 0; });

        const auto detrended = detrend<double>(ys);

        CHECK(rmse<double>(detrended, ys_ref) < 0.5);
    }

    SECTION("std::array")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        const auto ys1 = map(f1, xs);
        const auto ys2 = map(f2, xs);

        const auto ys_ref = from_function(n, [](int _)
                                          { return 0; });

        auto ys = map(plus<double, double>, ys1, ys2);

        auto detrended = detrend<double>(ys);

        CHECK(rmse<double>(detrended, ys_ref) < 0.5);
    }

    SECTION("Vector")
    {
        const auto xs = from_function(IntegralConst<int, n>{}, id<int>);

        const auto ys1 = map(f1, xs);
        const auto ys2 = map(f2, xs);

        const auto ys_ref = from_function(n, [](int _)
                                          { return 0; });

        const auto ys = map(plus<double, double>, ys1, ys2);

        const auto detrended = detrend<double>(ys);

        CHECK(rmse<double>(detrended, ys_ref) < 0.5);
    }
}

#endif