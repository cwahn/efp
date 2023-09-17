#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include <cmath>

#include "prelude.hpp"

namespace efp
{
    template <typename A, typename B>
    constexpr A max(const A &lhs, const B &rhs)
    {
        return max_v(lhs, rhs);
    }

    template <typename A, typename B>
    constexpr A min(const A &lhs, const B &rhs)
    {
        return min_v(lhs, rhs);
    }

    template <typename A, typename B>
    constexpr A plus(const A &lhs, const B &rhs)
    {
        return lhs + rhs;
    }

    template <typename A, typename B>
    constexpr A minus(const A &lhs, const B &rhs)
    {
        return lhs - rhs;
    }

    template <typename A, typename B>
    constexpr A times(const A &lhs, const B &rhs)
    {
        return lhs * rhs;
    }

    template <typename A, typename B>
    constexpr A devide(const A &lhs, const B &rhs)
    {
        return lhs / rhs;
    }

    template <typename A>
    constexpr A square(const A &a)
    {
        return a * a;
    }

    template <typename A>
    constexpr A sqrt(const A &a)
    {
        return std::sqrt(a);
    }

    template <typename A>
    constexpr A sin(const A &a)
    {
        return std::sin(a);
    }

    template <typename A>
    constexpr A cos(const A &a)
    {
        return std::cos(a);
    }

    template <typename A>
    constexpr A tan(const A &a)
    {
        return std::tan(a);
    }

    template <typename A>
    constexpr A abs(const A &a)
    {
        return a < 0 ? -a : a;
    }

    template <typename A, typename B>
    constexpr A is_approx(const A &lhs, const B &rhs)
    {
        using MorePrecise_t = Conditional_t<(sizeof(A) > sizeof(B)), A, B>;
        return abs(static_cast<MorePrecise_t>(lhs) - static_cast<MorePrecise_t>(rhs)) <= NumericLimits<MorePrecise_t>::epsilon();
    }

    // Reducing

    template <typename SeqA>
    constexpr Element_t<SeqA> maximum(const SeqA &as)
    {
        return foldl(max, NumericLimits<Element_t<SeqA>>::min(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> minimum(const SeqA &as)
    {
        return foldl(min, NumericLimits<Element_t<SeqA>>::max(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> max_min(const SeqA &as)
    {
        return maximum(as) - minimum(as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> sum(const SeqA &as)
    {
        return foldl(plus, 0, as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> product(const SeqA &as)
    {
        return foldl(times, 1, as);
    }
}

#endif