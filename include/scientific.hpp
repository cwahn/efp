#ifndef SCIENTIFIC_HPP_
#define SCIENTIFIC_HPP_

#include "prelude.hpp"
#include "numeric.hpp"

// todo Make default

namespace efp
{
    template <typename R, typename A>
    constexpr R mean(const Seq<A> &as)
    {
        return sum(as) / (R)length(as);
    }

    template <typename R, typename A>
    constexpr R rms(const Seq<A> &as)
    {
        return sqrt<R>(mean<R>(map(square<Element<A>>, as)));
    }

    template <typename R, typename A, typename B>
    R sse(const Seq<A> &as, const Seq<B> &bs)
    {
        const auto square_error = [](const Element<A> &a, const Element<B> &b)
        { return square(a - b); };

        return sum(map(square_error, as, bs));
    }

    template <typename R, typename A, typename B>
    R mse(const Seq<A> &as, const Seq<B> &bs)
    {
        const auto square_error = [](const Element<A> &a, const Element<B> &b)
        { return square(a - b); };

        return mean<R>(map(square_error, as, bs));
    }

    template <typename R, typename A, typename B>
    R rmse(const Seq<A> &as, const Seq<B> &bs)
    {
        return sqrt(mse<R>(as, bs));
    }

    template <typename R, typename A, typename B>
    R nrmse_mean(const Seq<A> &as, const Seq<B> &bs)
    {
        return rmse<R>(as, bs) / (R)mean<R>(bs);
    }

    template <typename R, typename A, typename B>
    R nrmse_max_min(const Seq<A> &as, const Seq<B> &bs)
    {
        return rmse<R>(as, bs) / (R)max_min(bs);
    }

    template <typename R, bool bessel_correction = false, typename A>
    R variance(const Seq<A> &as)
    {
        const auto as_mean = mean<R>(as);

        auto square_deviation = [&](const Element<A> &x)
        { return square(x - as_mean); };

        return bessel_correction
                   ? sum(map(square_deviation, as)) / (R)(length(as) - 1)
                   : sum(map(square_deviation, as)) / (R)length(as);
    }

    template <typename R, bool bessel_correction = false, typename A>
    R standard_deviation(const Seq<A> &as)
    {
        return sqrt(variance<R, bessel_correction>(as));
    }

    template <typename R, bool bessel_correction = false, typename A, typename B>
    R covariance(const Seq<A> &as, const Seq<B> &bs)
    {
        const auto as_mean = mean<R>(as);
        const auto bs_mean = mean<R>(bs);

        const auto covar = [&](const Element<A> &a, const Element<B> &b)
        { return (a - as_mean) * (b - bs_mean); };

        return bessel_correction
                   ? sum(map(covar, as, bs)) / (R)(length(as) - 1)
                   : sum(map(covar, as, bs)) / (R)length(as);
    }

    template <typename R, bool bessel_correction = false, typename A, typename B>
    R correlation(const Seq<A> &as, const Seq<B> &bs)
    {
        return covariance<R, bessel_correction>(as, bs) /
               (standard_deviation<R, bessel_correction>(as) * standard_deviation<R, bessel_correction>(bs));
    }

    template <typename R, bool bessel_correction = false, bool adjusted = false, typename A>
    R autocovariance(const Seq<A> &as, const int &lag)
    {
        const auto as_mean = mean<R>(as);
        const auto n = length(as);
        const int sum_length = n - lag;
        R summation = 0;

        for_index(
            [&](const int &i)
            { summation += (as[i] - as_mean) * (as[i + lag] - as_mean); },
            sum_length);

        if (adjusted)
        {
            return bessel_correction
                       ? summation / (R)(n - lag - 1)
                       : summation / (R)(n - lag);
        }
        else
        {
            return bessel_correction
                       ? summation / (R)(n - 1)
                       : summation / (R)n;
        }
    }

    template <typename R, bool bessel_correction = false, bool adjusted = false, typename A>
    R autocorrelation(const Seq<A> &as, const int &lag)
    {
        return autocovariance<R, bessel_correction, adjusted>(as, lag) /
               variance<R, bessel_correction>(as);
    }

    template <typename R, typename A>
    NAryReturn<R, A> remove_dc(const Seq<A> &as)
    {
        const auto as_mean = mean<R>(as);

        const auto minus_mean_as = [&](const Element<A> &x)
        { return (R)x - as_mean; };

        return map(minus_mean_as, as);
    }

    template <typename R, typename A, typename B>
    std::tuple<R, R> linear_regression(const Seq<A> &as, const Seq<B> &bs)
    {
        const auto as_mean = mean<R>(as);
        const auto bs_mean = mean<R>(bs);

        const auto minus_mean_as = [&](const Element<A> &x)
        { return (R)x - as_mean; };
        const auto minus_mean_bs = [&](const Element<B> &x)
        { return (R)x - bs_mean; };

        const auto a_deviations = map(minus_mean_as, as);
        const auto b_deviations = map(minus_mean_bs, bs);

        const auto ss_ab = sum(map(times<R, R>, a_deviations, b_deviations));
        const auto ss_aa = sum(map(square<R>, a_deviations));

        const auto beta_1 = ss_ab / ss_aa;
        const auto beta_2 = bs_mean - (beta_1 * as_mean);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename A>
    std::tuple<R, R> linear_regression_with_index(const Seq<A> &as)
    {
        const int n = length(as);

        const auto mean_is = (n - 1) / 2.;
        const auto as_mean = mean<R>(as);

        const auto get_ss_ia = [&](const int &i, const Element<A> &a)
        { return (i - mean_is) * ((R)a - as_mean); };

        const auto ss_ia = sum(map_with_index(get_ss_ia, as));
        const auto ss_ii = n * (n * n - 1) / 12.;

        const auto beta_1 = ss_ia / ss_ii;
        const auto beta_2 = as_mean - (beta_1 * mean_is);

        return std::make_tuple(beta_1, beta_2);
    }

    template <typename R, typename A>
    NAryReturn<R, A> detrend(const Seq<A> &as)
    {
        const auto betas = linear_regression_with_index<R>(as);
        const auto beta_1 = std::get<0>(betas);
        const auto beta_2 = std::get<1>(betas);

        const auto detrend_elem = [&](const Element<A> &i, const Element<A> &x)
        { return (R)x - (beta_1 * (R)i + beta_2); };

        return map_with_index(detrend_elem, as);
    }
}

#endif