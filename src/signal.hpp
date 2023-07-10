#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "prelude.hpp"
#include "numeric.hpp"
// ! temp
// #include "arduino_debug.hpp"

template <typename A, typename B>
double sse(A &&as, B &&bs)
{
    static auto square_error = [](ElementType_t<A> a, ElementType_t<A> b)
    {
        auto error = a - b;
        return error * error;
    };

    return sum(fmap(square_error, as, bs));
}

template <typename A, typename B>
double mse(A &&as, B &&bs)
{
    if constexpr (AreAllStatic<A, B>::value)
    {
        return sse(as, bs) / double(MinStaticCapacity<A, B>::value);
    }

    else
    {
        return sse(as, bs) / double(min_length(as, bs));
    }
}

template <typename A, typename B>
double rms(A &&as, B &&bs)
{
    return sqrt(mse(as, bs));
}

template <typename A, typename B>
double nrms_mean(A &&as, B &&bs)
{
    return rms(as, bs) / double(mean(bs));
}

template <typename A, typename B>
double nrms_max_min(A &&as, B &&bs)
{
    return rms(as, bs) / double(max_min(bs));
}

template <typename A>
double variance(A &&xs)
{
    double x_mean = mean(xs);

    auto minus_x_mean = [&](ElementType_t<A> x)
    {
        return x - x_mean;
    };

    auto deviations = fmap(minus_x_mean, xs);
    return sum(deviations) / double(length(xs));
}

template <typename A>
double standard_deviation(A &&xs)
{
    return sqrt(variance(xs));
}

template <typename A, typename B>
double covariance(A &&xs, B &&ys)
{
    double x_mean = mean(xs);
    double y_mean = mean(ys);

    auto covar = [&](ElementType_t<A> x, ElementType_t<B> y)
    {
        return (x - x_mean) * (y - y_mean);
    };

    return fmap(covar, xs, ys) / double(length(xs));
}

template <typename A, typename B>
double correlation(A &&xs, B &&ys)
{
    double x_standard_deviation = standard_deviation(xs);
    double y_standard_deviation = standard_deviation(ys);
    double xy_covariance = covariance(xs, ys);

    return xy_covariance / (x_standard_deviation * y_standard_deviation);
}

template <typename A>
double auto_covariance(A &&xs, uint lag)
{
    double x_mean = mean(xs);
    size_t xs_len = length(xs);
    double sum_length = xs_len - lag;

    double summation = 0;
    for (int i = 0; i < sum_length; ++i)
    {
        summation += (xs[i] - x_mean) * (xs[i + lag] - x_mean);
    }

    return summation / double(sum_length);
}

template <typename A>
FmapIterable_t<double, A> auto_covariance_function(A &&xs)
{
    auto auto_covar = [&](int i)
    {
        return auto_covariance(xs, i);
    };

    if constexpr (IsStatic<A>::value)
    {
        constexpr size_t length = StaticCapacity<A>::value;

        // ? Maybe can do this as constexpr
        StaticArray<int, length> idxs;
        std::iota(std::begin(idxs), std::end(idxs), 1);

        return fmap(auto_covar, idxs);
    }
    else
    {
        size_t length = length(xs);

        auto idxs = arange<std::vector<int>>(0, int(length), 1);

        return fmap(auto_covar, idxs);
    }
}

template <typename A>
FmapIterable_t<double, A> auto_corelation_function(A &&xs)
{
    auto x_auto_covariance_function = auto_covariance_function(xs);
    double x_variance = variance(xs);

    auto div_x_var = [&](ElementType_t<A> x)
    {
        return x / x_variance;
    };

    return fmap(div_x_var, x_auto_covariance_function);
}

template <typename A>
double auto_correlation(A &&xs, uint lag)
{
    double x_auto_covariance = auto_covariance(xs, lag);
    double x_variance = variance(xs);

    return x_auto_covariance / x_variance;
}

template <typename A>
FmapIterable_t<double, A> remove_dc(A &&xs)
{
    double x_mean = mean(xs);
    auto minus_x_mean = [&](ElementType_t<A> x)
    {
        return double(x) - x_mean;
    };

    return fmap(minus_x_mean, xs);
}

template <typename A, typename B>
std::tuple<double, double> linear_regression(A &&xs, B &&ys)
{
    double x_mean = mean(xs);
    double y_mean = mean(ys);

    auto minus_x_mean = [&](double x)
    {
        return x - x_mean;
    };

    auto minus_y_mean = [&](double y)
    {
        return y - y_mean;
    };

    auto x_deviations = fmap(minus_x_mean, xs);
    auto y_deviations = fmap(minus_y_mean, ys);

    double ss_xy = sum(fmap(times<double>, x_deviations, y_deviations));
    double ss_xx = sum(fmap(square<double>, x_deviations));

    double beta_1 = ss_xy / ss_xx;
    double beta_2 = y_mean - (beta_1 * x_mean);

    return std::make_tuple(beta_1, beta_2);
}

template <typename A>
FmapIterable_t<double, A> detrend(A &&xs)
{
    auto is = arange<RemoveReference_t<A>>(0, length(xs), 1);

    double beta_1, beta_2;
    std::tie(beta_1, beta_2) = linear_regression(is, xs);

    auto remove_trend = [&](ElementType_t<A> i, ElementType_t<A> x)
    {
        return x - (beta_1 * i + beta_2);
    };

    return fmap(remove_trend, is, xs);
}

#endif