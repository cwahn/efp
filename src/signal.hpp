#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#include "pure.hpp"
#include "math.hpp"

template <typename A>
std::tuple<double, double> linear_regression(A &xs, A &ys)
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

#endif
