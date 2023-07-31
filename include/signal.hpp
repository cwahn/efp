#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "prelude.hpp"
#include "numeric.hpp"

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
        const auto square_error = [](const Element_t<SeqA> &a, const Element_t<SeqB> &b)
        {
            const auto error = a - b;
            return error * error;
        };

        return sum(map(square_error, as, bs));
    }

    template <typename R, typename SeqA, typename SeqB>
    R mse(const SeqA &as, const SeqB &bs)
    {
        const auto square_error = [](const Element_t<SeqA> &a, const Element_t<SeqB> &b)
        {
            const auto error = a - b;
            return error * error;
        };

        return mean<R>(map(square_error, as, bs));
    }

    template <typename R, typename SeqA, typename SeqB>
    R rmse(const SeqA &as, const SeqB &bs)
    {
        return sqrt(mse<R>(as, bs));
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
        const auto mean_as = mean<R>(as);

        auto minus_mean_as = [&](const Element_t<SeqA> &x)
        {
            return x - mean_as;
        };

        const auto a_deviations = map(minus_mean_as, as);
        return sum(a_deviations) / (R)length(as);
    }

    template <typename R, typename SeqA>
    R standard_deviation(const SeqA &as)
    {
        return sqrt(variance<R>(as));
    }

    template <typename R, typename SeqA, typename SeqB>
    R covariance(const SeqA &as, const SeqB &bs)
    {
        const auto mean_as = mean<R>(as);
        const auto mean_bs = mean<R>(bs);

        const auto covar = [&](const Element_t<SeqA> &a, const Element_t<SeqB> &b)
        {
            return (a - mean_as) * (b - mean_bs);
        };

        return sum(map(covar, as, bs)) / (R)length(as);
    }

    template <typename R, typename SeqA, typename SeqB>
    R correlation(const SeqA &as, const SeqB &bs)
    {
        return covariance<R>(as, bs) / (standard_deviation<R>(as) * standard_deviation<R>(bs));
    }

    template <typename R, typename SeqA>
    R autocovariance(const SeqA &as, const int &lag)
    {
        const auto mean_as = mean<R>(as);
        const int sum_length = length(as) - lag;

        R summation = 0;

        for_index(
            [&](const int &i)
            {
                summation += (as[i] - mean_as) * (as[i + lag] - mean_as);
            },
            sum_length);

        return summation / (R)sum_length;
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> autocovariance_function(const SeqA &as)
    {
        const auto auto_covar = [&](const int &i, const Element_t<SeqA> &_)
        {
            return autocovariance<R>(as, i);
        };

        return map_with_index(auto_covar, as);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> autocorrelation_function(const SeqA &as)
    {
        const auto variance_as = variance<R>(as);
        const auto div_var_as = [&](const Element_t<SeqA> &x)
        {
            return x / variance_as;
        };

        return map(div_var_as, as);
    }

    template <typename R, typename SeqA>
    R autocorrelation(const SeqA &as, const int &lag)
    {
        return autocovariance<R>(as, lag) / variance<R>(as);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> remove_dc(const SeqA &as)
    {
        const auto mean_as = mean<R>(as);

        const auto minus_mean_as = [&](const Element_t<SeqA> &x)
        {
            return (R)x - mean_as;
        };

        return map(minus_mean_as, as);
    }

    template <typename R, typename SeqA, typename SeqB>
    std::tuple<R, R> linear_regression(const SeqA &as, const SeqB &bs)
    {
        const auto mean_as = mean<R>(as);
        const auto mean_bs = mean<R>(bs);

        const auto minus_mean_as = [&](const Element_t<SeqA> &x)
        {
            return (R)x - mean_as;
        };

        const auto minus_mean_bs = [&](const Element_t<SeqB> &x)
        {
            return (R)x - mean_bs;
        };

        const auto a_deviations = map(minus_mean_as, as);
        const auto b_deviations = map(minus_mean_bs, bs);

        const auto ss_ab = sum(map(times<R, R>, a_deviations, b_deviations));
        const auto ss_aa = sum(map(square<R>, a_deviations));

        const auto beta_1 = ss_ab / ss_aa;
        const auto beta_2 = mean_bs - (beta_1 * mean_as);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename SeqA>
    std::tuple<R, R> linear_regression_with_index(const SeqA &as)
    {
        const int n = length(as);

        const auto mean_is = (n - 1) / 2.;
        const auto mean_as = mean<R>(as);

        const auto get_ss_ia = [&](const int &i, const Element_t<SeqA> &a)
        {
            return (i - mean_is) * ((R)a - mean_as);
        };

        const auto ss_ia = sum(map_with_index(get_ss_ia, as));
        const auto ss_ii = n * (n * n - 1) / 12.;

        const auto beta_1 = ss_ia / ss_ii;
        const auto beta_2 = mean_as - (beta_1 * mean_is);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename SeqA>
    MapSequence_t<R, SeqA> detrend(const SeqA &as)
    {
        const auto betas = linear_regression_with_index<R>(as);

        const auto beta_1 = std::get<0>(betas);
        const auto beta_2 = std::get<1>(betas);

        const auto detrend_elem = [&](const Element_t<SeqA> &i, const Element_t<SeqA> &x)
        {
            return (R)x - (beta_1 * (R)i + beta_2);
        };

        return map_with_index(detrend_elem, as);
    }
}

#endif