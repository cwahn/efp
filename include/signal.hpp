#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "prelude.hpp"
#include "numeric.hpp"
// ! temp
// #include "arduino_debug.hpp"

namespace efp
{
    template <typename R, typename SeqA>
    constexpr R mean(const SeqA &as)
    {
        return sum(as) / (R)length(as);
    }

    template <typename R, typename SeqA>
    constexpr R rms(const SeqA &as)
    {
        return sqrt<R>(mean<R>(map(square<Element_t<SeqA>>, as)));
    }

    template <typename R, typename SeqA, typename SeqB>
    R sse(const SeqA &as, const SeqB &bs)
    {
        const auto square_error = [](Element_t<SeqA> a, Element_t<SeqB> b)
        {
            const auto error = a - b;
            return error * error;
        };

        return sum(map(square_error, as, bs));
    }

    template <typename R, typename SeqA, typename SeqB>
    R mse(const SeqA &as, const SeqB &bs)
    {
        const auto square_error = [](Element_t<SeqA> a, Element_t<SeqB> b)
        {
            const auto error = a - b;
            return error * error;
        };

        return mean<R>(map(square_error, as, bs));
    }

    template <typename R, typename SeqA, typename SeqB>
    R rmse(const SeqA &as, const SeqB &bs)
    {
        auto mse_value = mse<R>(as, bs);
        return sqrt<R>(mse_value);
    }

    template <typename R, typename SeqA, typename SeqB>
    R nrmse_mean(const SeqA &as, const SeqB &bs)
    {
        return rmse<R>(as, bs) / (R)mean<R>(bs);
    }

    template <typename R, typename SeqA, typename SeqB>
    R nrmse_max_min(const SeqA &as, const SeqB &bs)
    {
        return rmse<R>(as, bs) / (R)max_min(bs);
    }

    template <typename R, typename SeqA>
    R variance(const SeqA &as)
    {
        const R a_mean = mean<R>(as);

        auto minus_a_mean = [&](Element_t<SeqA> x)
        {
            return x - a_mean;
        };

        const auto a_deviations = map(minus_a_mean, as);
        return sum(a_deviations) / double(length(as));
    }

    template <typename R, typename SeqA>
    R standard_deviation(const SeqA &as)
    {
        return sqrt(variance<R>(as));
    }

    template <typename R, typename SeqA, typename SeqB>
    R covariance(const SeqA &as, const SeqB &bs)
    {
        const double a_mean = mean<R>(as);
        const double b_mean = mean<R>(bs);

        const auto covar = [&](Element_t<SeqA> a, Element_t<SeqB> b)
        {
            return (a - a_mean) * (b - b_mean);
        };

        return sum(map(covar, as, bs)) / (R)length(as);
    }

    template <typename R, typename SeqA, typename SeqB>
    R correlation(const SeqA &as, const SeqB &bs)
    {
        return covariance<R>(as, bs) / (standard_deviation<R>(as) * standard_deviation<R>(bs));
    }

    template <typename R, typename SeqA>
    R auto_covariance(const SeqA &as, const int lag)
    {
        const R a_mean = mean<R>(as);
        const int as_len = length(as);
        const int sum_length = as_len - lag;

        R summation = 0;

        for_index(
            [&](const int i)
            {
                summation += (as[i] - a_mean) * (as[i + lag] - a_mean);
            },
            sum_length);

        return summation / (R)sum_length;
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> auto_covariance_function(const SeqA &as)
    {
        const auto auto_covar = [&](const int i, Element_t<SeqA> _)
        {
            return auto_covariance<R>(as, i);
        };

        return map_with_index(auto_covar, as);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> auto_corelation_function(const SeqA &as)
    {
        const R a_variance = variance<R>(as);
        const auto div_a_var = [&](Element_t<SeqA> x)
        {
            return x / a_variance;
        };

        return map(div_a_var, auto_covariance_function<R>(as));
    }

    template <typename R, typename SeqA>
    R auto_correlation(const SeqA &as, const int lag)
    {
        return auto_covariance<R>(as, lag) / variance<R>(as);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> remove_dc(const SeqA &as)
    {
        const R a_mean = mean<R>(as);

        const auto minus_a_mean = [&](Element_t<SeqA> x)
        {
            return (R)x - a_mean;
        };

        return map(minus_a_mean, as);
    }

    template <typename R, typename SeqA, typename SeqB>
    std::tuple<R, R> linear_regression(const SeqA &as, const SeqB &bs)
    {
        const R a_mean = mean<R>(as);
        const R b_mean = mean<R>(bs);

        const auto minus_a_mean = [&](const Element_t<SeqA> x)
        {
            return (R)x - a_mean;
        };

        const auto minus_b_mean = [&](const Element_t<SeqB> x)
        {
            return (R)x - b_mean;
        };

        const auto a_deviations = map(minus_a_mean, as);
        const auto b_deviations = map(minus_b_mean, bs);

        const R ss_ab = sum(map(times<R, R>, a_deviations, b_deviations));
        const R ss_aa = sum(map(square<R>, a_deviations));

        const R beta_1 = ss_ab / ss_aa;
        const R beta_2 = b_mean - (beta_1 * a_mean);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename SeqA>
    std::tuple<R, R> linear_regression_with_index(const SeqA &as)
    {
        const int n = length(as);

        const R i_mean = (n - 1) / 2.;
        const R a_mean = mean<R>(as);

        const auto get_ss_ia = [&](const int i, const Element_t<SeqA> a)
        {
            return (i - i_mean) * ((R)a - a_mean);
        };

        const R ss_ia = sum(map_with_index(get_ss_ia, as));
        const R ss_ii = n * (n * n - 1) / 12.;

        const R beta_1 = ss_ia / ss_ii;
        const R beta_2 = a_mean - (beta_1 * i_mean);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> detrend(const SeqA &as)
    {
        const std::tuple<R, R> betas = linear_regression_with_index<R>(as);

        const R beta_1 = std::get<0>(betas);
        const R beta_2 = std::get<1>(betas);

        const auto remove_trend = [&](Element_t<SeqA> i, Element_t<SeqA> x)
        {
            return (R)x - (beta_1 * (R)i + beta_2);
        };

        return map_with_index(remove_trend, as);
    }
}

#endif