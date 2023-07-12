#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "prelude.hpp"
#include "numeric.hpp"
// ! temp
// #include "arduino_debug.hpp"

template <typename SeqA, typename SeqB>
constexpr double sse(const SeqA &as, const SeqB &bs)
{
    const auto square_error = [](Element_t<SeqA> a, Element_t<SeqB> b)
    {
        const auto error = a - b;
        return error * error;
    };

    return sum(map(square_error, as, bs));
}

template <typename SeqA, typename SeqB>
constexpr double mse(const SeqA &as, const SeqB &bs)
{
    if constexpr (AreAllStaticCapacity<SeqA, SeqB>::value)
    {
        return sse(as, bs) / double(MinStaticCapacity<SeqA, SeqB>::value);
    }

    else
    {
        return sse(as, bs) / double(min_length(as, bs));
    }
}

template <typename SeqA, typename SeqB>
constexpr double rms(const SeqA &as, const SeqB &bs)
{
    return sqrt(mse(as, bs));
}

template <typename SeqA, typename SeqB>
constexpr double nrms_mean(const SeqA &as, const SeqB &bs)
{
    return rms(as, bs) / double(mean(bs));
}

template <typename SeqA, typename SeqB>
constexpr double nrms_max_min(const SeqA &as, const SeqB &bs)
{
    return rms(as, bs) / double(max_min(bs));
}

template <typename SeqA>
constexpr double variance(const SeqA &as)
{
    const double a_mean = mean(as);

    auto minus_a_mean = [&](Element_t<SeqA> x)
    {
        return x - a_mean;
    };

    const auto a_deviations = map(minus_a_mean, as);
    return sum(a_deviations) / double(length(as));
}

template <typename SeqA>
constexpr double standard_deviation(const SeqA &as)
{
    return sqrt(variance(as));
}

template <typename SeqA, typename SeqB>
constexpr double covariance(const SeqA &as, const SeqB &bs)
{
    const double a_mean = mean(as);
    const double b_mean = mean(bs);

    const auto covar = [&](Element_t<SeqA> a, Element_t<SeqB> b)
    {
        return (a - a_mean) * (b - b_mean);
    };

    return map(covar, as, bs) / double(length(as));
}

template <typename SeqA, typename SeqB>
constexpr double correlation(const SeqA &as, const SeqB &bs)
{
    const double a_standard_deviation = standard_deviation(as);
    const double b_standard_deviation = standard_deviation(bs);
    const double ab_covariance = covariance(as, bs);

    return ab_covariance / (a_standard_deviation * b_standard_deviation);
}

template <typename SeqA>
constexpr double auto_covariance(const SeqA &as, uint lag)
{
    const double a_mean = mean(as);
    const size_t as_len = length(as);
    const double sum_length = as_len - lag;

    double summation = 0;

    for (int i = 0; i < sum_length; ++i)
    {
        summation += (as[i] - a_mean) * (as[i + lag] - a_mean);
    }

    return summation / double(sum_length);
}

// ! Temporary nonconst expr
template <typename SeqA>
MapSequance_t<double, SeqA> auto_covariance_function(const SeqA &as)
{
    const auto auto_covar = [&](int i)
    {
        return auto_covariance(as, i);
    };

    if constexpr (IsStaticCapacity<SeqA>::value)
    {
        constexpr size_t result_length = StaticCapacity<SeqA>::value;

        // ? Maybe can do this as constexpr
        StaticArray<int, result_length> idxs;
        std::iota(std::begin(idxs), std::end(idxs), 1);

        return map(auto_covar, idxs);
    }
    else
    {
        const size_t result_length = length(as);

        const auto idxs = arange<DynamicVector<int>>(0, result_length, 1);

        return map(auto_covar, idxs);
    }
}

template <typename SeqA>
constexpr MapSequance_t<double, SeqA> auto_corelation_function(const SeqA &as)
{
    const auto a_auto_covariance_function = auto_covariance_function(as);
    const double a_variance = variance(as);

    const auto div_a_var = [&](Element_t<SeqA> x)
    {
        return x / a_variance;
    };

    return map(div_a_var, a_auto_covariance_function);
}

template <typename SeqA>
constexpr double auto_correlation(const SeqA &as, const uint lag)
{
    const double a_auto_covariance = auto_covariance(as, lag);
    const double a_variance = variance(as);

    return a_auto_covariance / a_variance;
}

template <typename SeqA>
constexpr MapSequance_t<double, SeqA> remove_dc(const SeqA &as)
{
    const double a_mean = mean(as);

    const auto minus_a_mean = [&](Element_t<SeqA> x)
    {
        return double(x) - a_mean;
    };

    return map(minus_a_mean, as);
}

template <typename SeqA, typename SeqB>
constexpr std::tuple<double, double> linear_regression(const SeqA &as, const SeqB &bs)
{
    const double a_mean = mean(as);
    const double b_mean = mean(bs);

    const auto minus_a_mean = [&](double x)
    {
        return x - a_mean;
    };

    const auto minus_b_mean = [&](double x)
    {
        return x - b_mean;
    };

    const auto a_deviations = map(minus_a_mean, as);
    const auto b_deviations = map(minus_b_mean, bs);

    const double ss_ab = sum(map(times<double>, a_deviations, b_deviations));
    const double ss_aa = sum(map(square<double>, a_deviations));

    const double beta_1 = ss_ab / ss_aa;
    const double beta_2 = b_mean - (beta_1 * a_mean);

    return std::make_tuple(beta_1, beta_2);
}

template <typename SeqA>
constexpr MapSequance_t<double, SeqA> detrend(const SeqA &as)
{
    auto is = arange<RemoveReference_t<SeqA>>(0, length(as), 1);

    // double beta_1, beta_2;
    std::tuple<double, double> betas = linear_regression(is, as);
    const double beta_1 = std::get<0>(betas);
    const double beta_2 = std::get<1>(betas);

    const auto remove_trend = [&](Element_t<SeqA> i, Element_t<SeqA> x)
    {
        return x - (beta_1 * i + beta_2);
    };

    return map(remove_trend, is, as);
}

#endif